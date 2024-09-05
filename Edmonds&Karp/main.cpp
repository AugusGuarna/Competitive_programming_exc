#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <cmath>

/*
 * Vamos a modelar el problema como un problema de matching máximo en un grafo bipartito.
 * El grafo bipartito en este caso es G/ V(G) = {voluntarios} U {talles de remeras} y E(G) = {v_i t_i / v_i pertenece al
 * conjunto de voluntarios, t_i al conjunto de talles de remeras y el voluntario v_i tiene como talle el talle t_i}.
 * Como lo vamos a plantear de esta forma lo vamos a resolver como un ejercicio de flujo máximo.
 * Mi red N va a tener V(N) = V(G) u {s, t}, E(N) = {s -> v_i/ v_i pertence al conjunto de voluntarios} u {t_i -> t/ t_i pertence al conjunto de talles}
 * u { v_i -> t_i/ v_i t_i era una arista de G} y su función de capacidades va a ser
 * c(e) = 1 si e = s -> v_i o t_i -> t
 * c(e) = #cantTallesDisponiblesEnStock si e = v_i -> t_i
 * Estas capacidades son porque a cada voluntario se le puede asignar una sola remera y porque queremos saber cuándo se han asignado
 * todas las remeras disponibles de un talle entre los voluntarios.
 * Para resolverlo vamos a utilizar el algoritmo de Edmonds & Karps. La implementación de este algoritmo fue la dada por Julián Braier en la
 * clase práctica de Flujo II. Implementación que si bien no subió al campus fue copiada en mi cuaderno de apuntes.
 *
 * La solución a este problema va a existir si existe un valor de flujo equivalente a mi cantidad de voluntarios porque eso quiere decir
 * que mi corte mínimo termina siendo s. Si mi corte mínimo termina siendo s quiere decir que pude asignar una remera a cada voluntario.
 *
 * Al grafo lo vamos a representar como una lista de adyacencias donde la posición 0 del vector va a ser la fuente, las posiciones
 * 1 a 6 los talles ordenados según XS, S, M, L, XL, XXL de la 6 hasta 6+cantVoluntarios la información de la cantidad de voluntarios y
 * finalmente en la posición 7 + cantVoluntarios + 1 va a estar la información del sumidero.
 *
 * A su vez vamos a representar en el grafo la misma red residual agregando una arista t_i -> v_i por cada arista v_i -> t_i que tengamos
 * y los flujos y capacidades los vamos a representar en matrices
 */

using namespace std;

const int inf = pow(10, 9);

tuple<bool, vector<int>> existeCaminoAumento(int s, int t, vector<vector<int>> &grafo, vector<vector<int>>& flujo, vector<vector<int>>& capacidades){
    vector<int> padres(grafo.size(), -1);
    padres[s] = s;
    queue<int> colaDePrio;
    colaDePrio.push(s);
    while(!colaDePrio.empty()){
        int v = colaDePrio.front();
        colaDePrio.pop();
        for(auto u : grafo[v]){
            if(flujo[v][u] < capacidades[v][u] && padres[u] == -1){
                padres[u] = v;
                colaDePrio.push(u);
            }
        }
    }
    return {padres[t] != -1, padres};
}



int calcularDelta(int s, int t, vector<vector<int>> &grafo, vector<vector<int>>& flujo, vector<vector<int>>& capacidades, vector<int>& caminoAumento){
    int delta = inf;
    for(; t != s; t = caminoAumento[t]){
        int predecesor = caminoAumento[t];
        delta = min(delta, capacidades[predecesor][t] - flujo[predecesor][t]);
    }
    return delta;
}

void aumentar_camino(int s, int t, vector<vector<int>> &grafo, vector<vector<int>>& flujo, vector<vector<int>>& capacidades, vector<int>& caminoAumento, int deltaCamino) {
    for (; t != s; t = caminoAumento[t]) {
        int predecesor = caminoAumento[t];
        flujo[predecesor][t] += deltaCamino;
        flujo[t][predecesor] -= deltaCamino;
    }
}

int aumentarFlujo(int s, int t, vector<vector<int>> &grafo, vector<vector<int>>& flujo, vector<vector<int>>& capacidades, vector<int>& caminoAumento){
    int deltaCamino = calcularDelta(s,t,grafo,flujo,capacidades,caminoAumento);

    aumentar_camino(s,t,grafo,flujo,capacidades,caminoAumento, deltaCamino);

    return deltaCamino;
}

int EdmondsKarp(int s, int t, vector<vector<int>> &grafo, vector<vector<int>>& flujo, vector<vector<int>>& capacidades){
    int flujoMax = 0;
    bool sigoIterando = true;
    while(sigoIterando){
        tuple<int, vector<int>> existeCamino = existeCaminoAumento(s, t, grafo, flujo, capacidades);
        sigoIterando = get<0>(existeCamino);
        if(sigoIterando){
            flujoMax += aumentarFlujo(s,t,grafo,flujo,capacidades, get<1>(existeCamino));
        }
    }
    return flujoMax;
}

bool encontrarDistribucion(vector<vector<int>> &grafo, vector<vector<int>>& flujo, vector<vector<int>>& capacidades, int cantVoluntarios){
    // Va a existir una distribución si el flujo máximo coincide con la cantidad de voluntarios
    int flujoMax =  EdmondsKarp(0, grafo.size()-1, grafo, flujo, capacidades);

    return flujoMax == cantVoluntarios;
}



int main() {
    int tamanoTestSuite;
    cin>>tamanoTestSuite;
    for(int i = 0; i < tamanoTestSuite; i++){
        int cantRemeras;
        int cantVoluntarios;
        while(cin>>cantRemeras>>cantVoluntarios) {
            // Aprovecho y armo el grafo ya con los inputs
            int cantTallesXRemera = cantRemeras / 6;
            /*
             * Cada talle va a tener un número asociado 1 = XS 2 = S y así hasta el 6
             */
            int tamanoGrafo = cantVoluntarios + 6 + 2;
            vector<vector<int>> grafo(tamanoGrafo, vector<int>());
            vector<vector<int>> capacidades(tamanoGrafo, vector<int>(tamanoGrafo));
            vector<vector<int>> flujo = capacidades;
            map<string, int> mapaTalles;
            string talleXs = "XS";
            string talleS = "S";
            string talleM = "M";
            string talleL = "L";
            string talleXL = "XL";
            string talleXXL = "XXL";
            mapaTalles[talleXs] = 1;
            mapaTalles[talleS] = 2;
            mapaTalles[talleM] = 3;
            mapaTalles[talleL] = 4;
            mapaTalles[talleXL] = 5;
            mapaTalles[talleXXL] = 6;

            /*
             * Para el grafo el vértice en la posición 0 va a ser la fuente y el vértice en la última posición el
             * sumidero
             */

            // Conecto a todos los voluntarios con la fuente
            for (int i = 1; i <= cantVoluntarios; i++) {
                grafo[0].push_back(6 + i);// Las primeras 6 posiciones van a ser los talles
                grafo[6+i].push_back(0); // Ya voy armando la red residual
                capacidades[0][6 + i] = 1;
            }
            // Conecto a todos los talles con el sumidero y defino un mapa de talles que por cada talle me da su número correspondiente
            for (int i = 1; i <= 6; i++) {
                grafo[i].push_back(tamanoGrafo - 1);
                grafo[tamanoGrafo - 1].push_back(i); // Armado red residual
                capacidades[i][tamanoGrafo - 1] = cantTallesXRemera;
            }

            string opcionTalle1;
            string opcionTalle2;
            // Agrego la información de cada voluntario y duplico por la red residual
            for (int i = 1; i <= cantVoluntarios; i++) {
                cin >> opcionTalle1 >> opcionTalle2;
                grafo[6 + i].push_back(mapaTalles[opcionTalle1]);
                grafo[mapaTalles[opcionTalle1]].push_back(6 + i);

                grafo[6 + i].push_back(mapaTalles[opcionTalle2]);
                grafo[mapaTalles[opcionTalle2]].push_back(6 + i);

                capacidades[6 + i][mapaTalles[opcionTalle1]] = 1;
                capacidades[6 + i][mapaTalles[opcionTalle2]] = 1;
            }

            bool existeUnaDistribucionDeTalles = encontrarDistribucion(grafo, flujo, capacidades, cantVoluntarios);
            if (existeUnaDistribucionDeTalles) {
                cout << "YES" << endl;
            } else {
                cout << "NO" << endl;
            }
        }
    }


    return 0;
}
