#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>

using namespace std;
const int inf = pow(10,8);

/*
 * Para resolver el problema lo vamos a modelar como un SRD. Para ello vamos a armarnos un digrafo pesado.
 * No obstante, a la hora de definir las incógnitas vamos a hacer una observación.
 * Supongamos que un input resulta siendo 2 3 < 0. Esto nos dice a_2 + a_3 + a_4 + a_5 < 0.
 * Sin embargo, lo podemos ver de esta otra manera a_1 + a_2 + a_3 + a_4 + a_5 - a_1 < 0. Si consideramos la suma desde a_1 hasta
 * a_j como Sj, podemos modelar 2 3 < 0 como la inecuación S_(2+3) - S_(2-1) < 0.
 * De esta forma nos vamos a armar el SRD. Para el caso donde tengamos solo la sumatoria S_1 vamos a agregar un nodo fantasma
 * que vendría a funcionar como S_0 y plantearemos la desigualdad en función de ellos.
 *
 * Las ecuaciones van a quedar así para los siguientes inputs
 * n_i s_j > k_ij = S_(n_i-1) - S_(n_i+s_j) <= - (k_ij-1)
 * n_i s_j < k_ij = S_(n_i+s_j) - S_(n_i-1)  <= k_ij-1
 *
 * Mas el caso especial para las inecuaciones que solo tengan S_1
 *
 * Dicho esto armaremos el SRD con un digrafo pesado D/
 * V(D) = {S_i / 0 <= 0 <= largo de la secuencia} U {fuente s}
 * E(D) = {S_j -> S_i sii S_i - S_j es una inecuación} U {s -> S_k para S_k perteneciente V(D)}
 * c: E(D) -> R / c(S_j -> S_i) = Kij donde Kij es la restricción que nos pasan como input
 *
 * Como solamente queremos saber si existe una secuencia que falsee basta con hacer Bellman Ford para checkear si existen
 * ciclos de peso negativo. Si existen, la solución no existe. Si no, existe solución. Por lo tanto, vamos a implementar
 * un Bellman Ford que solo devuelva si hay o no ciclo negativo, no más que eso.
 *
 * Obs: si nos dan una ecuación que repite incógnitas entonces mucho no importa porque en las iteraciones de Bellman Ford
 * se va a quedar con la de restricción kij mínima
 */


//Estamos usando una implementación del algoritmo de Bellman Ford basada en el pseudocódigo que dio el Dr. Soulignac en clase

bool algoritmoBellmanFord(vector<vector<tuple<int,int>>> grafo, int v){
    vector<int> diccCostos(grafo.size(), inf);
    diccCostos[v] = 0;
    bool modificado = true;
    for(int i = 0; i < grafo.size() && modificado; i++){
        modificado = false;
        for(int j = 0; j < grafo.size(); j++){
            for(tuple<int,int> t : grafo[j]){
                if(diccCostos[j] + get<1>(t) < diccCostos[get<0>(t)]){
                    diccCostos[get<0>(t)] = diccCostos[j] + get<1>(t);
                    modificado = true;
                }
            }
        }
    }
    return modificado;
}


vector<vector<tuple<int,int>>> armarSRD(vector<tuple<int,int,string,int>> &infoSistema, int cantVariables){
    vector<vector<tuple<int,int>>> SRD(cantVariables + 2, vector<tuple<int,int>>());
    // Agregamos dos posiciones más, una para la fuente s desde la cual correremos BF y otro nodo para el caso S_1 que ya hablamos
    for(int j = 1; j <= cantVariables; j++){
        SRD[0].push_back(make_tuple(j,0));
    }
    for(int i = 0; i < infoSistema.size(); i++){
        int n_i;
        int s_i;
        string o_i;
        int k_i;
        tie(n_i, s_i, o_i, k_i) = infoSistema[i];
        if(o_i == "gt"){
            if(n_i == 1){
                SRD[n_i+s_i].push_back(make_tuple((SRD.size()-1), -(k_i+1)));
            } else {
                SRD[n_i + s_i].push_back(make_tuple(n_i - 1, -(k_i+1)));
            }
        } else {
            if(n_i == 1){
                SRD[(SRD.size()-1)].push_back(make_tuple(n_i+s_i, k_i-1));
            } else {
                SRD[n_i-1].push_back(make_tuple(n_i + s_i, k_i - 1));
            }
        }
    }
    return SRD;
}

bool resolucionDeSistemasDelRey(vector<tuple<int,int,string,int>> &infoSistema, int cantVariables){
    /*
     * 1. Armo el digrafo que representa mi SRD
     */
    vector<vector<tuple<int,int>>> SRD = armarSRD(infoSistema, cantVariables);
    /*
     * 2. Corro Bellman Ford sobre el digrafo y veo si tiene ciclos negativos o no. Si tiene no existe solución
     * Si no tiene la solución existe
     */
    bool resultadoBellmanFord = algoritmoBellmanFord(SRD, 0);

    return !resultadoBellmanFord;

}

int main() {
    int cantVariables;
    int cantSistemas;
    while(cin >> cantVariables){
        if(cantVariables != 0){
            cin >> cantSistemas;
            int s_i;
            int n_i;
            string o_i;
            int k_i;
            vector<tuple<int,int,string,int>> infoSistema;
            for(int i = 0; i < cantSistemas; i++){
                cin >> s_i >> n_i >> o_i >> k_i;
                infoSistema.push_back(make_tuple(s_i, n_i, o_i, k_i));
            }
            bool existeSol = resolucionDeSistemasDelRey(infoSistema, cantVariables);
            if(existeSol){
                cout << "lamentable kingdom" <<endl;
            } else{
                cout << "successful conspiracy" <<endl;
            }
        }

    }
    return 0;
}
