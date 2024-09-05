#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>
#include <limits>
#include <cmath>
#include <set>

using namespace std;
const int inf = pow(2,25);
/*
 * Para resolver el problema vamos a trabajar con un grafo pesado G (sino podríamos tener un digrafo con aristas paralelas)
 * V(G) = {para cada ascensor vamos a tener los pisos en los que se detiene} U {s}
 * E(G) = {v_i w_i / el ascensor i va desde v_i hasta w_i} U {conexiones entre ascensores de un mismo piso} U {s--> 0 para todo ascensor
 * que pare en el piso 0}
 * Este conjunto {conexiones entre ascensores de un mismo piso es decir} se representa considerando que si el ascensor j y el ascensor i
 * se detienen en el piso l-ésimo entonces para los nodos que representen el piso l-ésimo para estos dos ascensores va a haber una arista que los conecte
 *
 * c(E(G)) = t que tarda el ascensor i de ir desde v_i hasta w_i
 * Con la excepción de que el costo de ir de s al piso 0 es 0
 *
 * Al grafo lo vamos a representar mediante una lista de adyacencias donde vamos a guardar el piso, el costo de ir a ese piso y el ascensor
 * que me lleva.
 * Luego para resolver el problema vamos a correr un dijkstra con algunas modificaciones desde s y fijarnos el costo de ir desde s al piso k.
 * Como s está conectado con costo 0 el costo de ir de s a k es el mismo que ir desde algún piso 0 a k.
 */

vector<vector<tuple<int,int,int>>> transformarAGrafo(vector<vector<int>>& mapaAscensores, vector<int>& costos, int alturaEdificio){
    vector<vector<tuple<int,int,int>>> grafo(alturaEdificio+2, vector<tuple<int,int,int>>());
    // Pedimos que el tamaño sea alturaEdificio + 2 para poder agregar el nodo s
    for(int i = 0; i < mapaAscensores.size(); i++){
        for(int j = 1; j < mapaAscensores[i].size(); j++){
            int costoMoverseDePiso = (mapaAscensores[i][j] - mapaAscensores[i][j-1])*costos[i];
            grafo[mapaAscensores[i][j-1]].push_back(make_tuple(mapaAscensores[i][j], costoMoverseDePiso,i));
            grafo[mapaAscensores[i][j]].push_back(make_tuple(mapaAscensores[i][j-1], costoMoverseDePiso,i));
        }
    }
    for(int i = 0; i < grafo[0].size(); i++){
        grafo[alturaEdificio+1].push_back(make_tuple(0,0, get<2>(grafo[0][i])));
    }
    return grafo;
}

// Esta implementación de Dijkstra está basada en la versión de cpalgorithms del algoritmo
vector<int> algoritmoDijkstra(vector<vector<tuple<int,int,int>>> &grafo, int v){
    vector<int> kDPM(v+1, inf);
    vector<int> padres(v+1, -1);

    set<tuple<int,int,int>> S;
    S.insert({0,v,-1});
    kDPM[v] = 0;

    while(!S.empty()) {

        int costoMoverme, pisoEnElQueEstoy, ascensorEnElQueEstaba;
        tie(costoMoverme, pisoEnElQueEstoy, ascensorEnElQueEstaba) = *(S.begin());
        S.erase(S.begin());

        for(int j = 0; j < grafo[pisoEnElQueEstoy].size(); j++){
            int pisoAlQueVoy = get<0>(grafo[pisoEnElQueEstoy][j]);
            int costo = get<1>(grafo[pisoEnElQueEstoy][j]);
            int ascensor = get<2>(grafo[pisoEnElQueEstoy][j]);
            if(ascensorEnElQueEstaba != ascensor && ascensorEnElQueEstaba != -1){
                costo = costo + 60;
            }
            // Consideramos ahora el costo de cambiar de piso porque antes no lo estabamos considerando en el grafo
            if(costoMoverme + costo < kDPM[pisoAlQueVoy]) {
                kDPM[pisoAlQueVoy] = costoMoverme + costo;
                S.insert(make_tuple(costoMoverme + costo, pisoAlQueVoy, ascensor));
                padres[pisoAlQueVoy] = pisoEnElQueEstoy;
                /*
                 * Hasta acá es el Djikstra normal, sin embargo, como pueden haber otros caminos para llegar al kesimo piso
                 * cambiando de ascensor vamos a necesitar tener todas las aristas que me cambien de ascensor (y para simplificarlo
                 * me quede con todas porque no es un grafo denso sino más bien ralo) para poder considerar el caso de que me convenga
                 * pegar el cambio
                 */
            } else {
                if(pisoAlQueVoy < pisoEnElQueEstoy && pisoEnElQueEstoy != v){ //Agrego las aristas que me hacen bajar a pisos no visitados y cuyo padre no es el vértice inicial
                    if(padres[pisoAlQueVoy] == -1){
                        S.insert(make_tuple(costoMoverme + costo, pisoAlQueVoy, ascensor));
                    }
                } else{
                    S.insert(make_tuple(costoMoverme + costo, pisoAlQueVoy, ascensor));
                }

            }
        }
    }
    return kDPM;
}


int rutaMasRapidaParaLlegarAlKesimoPiso(vector<vector<int>>& mapaAscensores, vector<int>& costos, int kesimopiso, int alturaEdificio){
    /*
    * 1. Armo mi grafo
    */
    vector<vector<tuple<int,int,int>>> grafo = transformarAGrafo(mapaAscensores, costos, alturaEdificio);

    /*
    * 2. Hago Dijkstra sobre mi grafo y tomo como vértice inicial el vértice s que lo almacenamos en la posición alturaEdificio + 1
    */
    vector<int> tiempoParaLlegarALosPisos = algoritmoDijkstra(grafo, alturaEdificio+1);
    /*
    * 3. Devuelvo el costo del ACM para el kesimopiso
    */
    return tiempoParaLlegarALosPisos[kesimopiso];
}

int main() {
    int cantidadAscensores;
    int pisoAlQueQueremosLlegar;
    while(cin>>cantidadAscensores>>pisoAlQueQueremosLlegar){
        bool existeSolucion = false;
        // Creo un diccionario para saber cuánto tarda un ascensor en moverse entre dos pisos adyacentes
        vector<int> diccionarioDeTardanzaDeAscensores(cantidadAscensores,0);
        int tiempoCambiarDePiso = 0;
        for(int i = 0; i < cantidadAscensores; i++){
            cin >> tiempoCambiarDePiso;
            diccionarioDeTardanzaDeAscensores[i] = tiempoCambiarDePiso;
        }
        // Creo un diccionario para saber a qué piso llega cada ascensor
        vector<vector<int>> pisosAlosQueLlegaCadaAscensor(cantidadAscensores, vector<int>());
        /*
         * Voy a levantar las líneas que corresponden a los pisos a los que llega cada ascensor con el comando
         * getline de c++ para evitar que se pise con otros casos de test. Si bien me lo devuelve como string
         * los voy a ir pasando a int y de paso checkeo si existe solucion
         */
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // Esta línea de código la saque de esta página porque no tenía ni idea como hacer para que me funcione el cin.ignore
        // https://www.tutorialspoint.com/what-is-the-use-of-cin-ignore-in-cplusplus

        string pisos;
        int pisoMaximo = 0;
        for(int i = 0; i < cantidadAscensores; i++){
            getline(cin, pisos);
            istringstream diccPisos(pisos); // Voy sacando los pisos de la string
            int numeroDePiso;
            while(diccPisos >> numeroDePiso){
                pisosAlosQueLlegaCadaAscensor[i].push_back(numeroDePiso);
                if(numeroDePiso == pisoAlQueQueremosLlegar && !existeSolucion){
                    existeSolucion = true;
                }
                if(pisoMaximo < numeroDePiso){
                    pisoMaximo = numeroDePiso;
                }
            }
        }
        int tiempoMinimoRequerido = rutaMasRapidaParaLlegarAlKesimoPiso
                (pisosAlosQueLlegaCadaAscensor, diccionarioDeTardanzaDeAscensores,
                 pisoAlQueQueremosLlegar, pisoMaximo);
        if(tiempoMinimoRequerido != inf && existeSolucion){
            cout << tiempoMinimoRequerido << endl;
        } else {
            cout << "IMPOSSIBLE" <<endl;
        }
    }
    return 0;

}