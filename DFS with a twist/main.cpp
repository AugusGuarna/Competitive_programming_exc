#include <iostream>
#include <utility>
#include <vector>
#include <list>

using namespace std;
using estaciones = pair<int,int>;
using listaDeEstaciones = list<estaciones>;
using mapaEstaciones = vector<list<int>>;
const int VERDE = 0;
const int AMARILLO = 1;
const int ROJO = 2;

/*
 * Para resolver el problema vamos a modelarlo usando grafos.
 * En este caso vamos a considerar un grafo G/ V(G) = estaciones de tren y E(G) = vías entre estaciones.
 * Si analizamos el enunciado vemos que lo que en el fondo nos pide el problema con esto de los pigeon values y demás
 * es que encontremos los puntos de articulación del grafo G. Para eso vamos a usar el algoritmo que vimos en la clase práctica
 * de recorrido de grafos dada por Martin Amster (turno mañana).
 * Si bien este algoritmo nos ayuda a encontrar aristas puentes, también nos va a ayudar a la detección de puntos de articulación.
 * Aquellos puntos de articulación van a ser o:
 * 1) aquellos vértices cuyas aristas padre en el árbol DFS T
 * (que se obtiene de implementar DFS en G) sean aristas puente o
 * 2) aquellos nodos que quizás estén cubiertos por una arista pero que sus hijos no lo estén. Si sus hijos no están cubiertos por una arista por más que su
 * padre lo esté si removemos el padre no tendremos otra forma de llegar a ellos. Es decir, si las aristas del padre a los hijos son puentes.
 * Hay que tener un cuidado igual: si bien la raíz del árbol no va a tener una arista padre, si tiene más de un hijo es un punto de articulación.
 */


mapaEstaciones TransformarAMapa(listaDeEstaciones &l1, int n){
    // Paso de lista de aristas a lista de adyacencias
    mapaEstaciones imperio(n);
    for(estaciones vias : l1){
        imperio[vias.first].push_back(vias.second);
        imperio[vias.second].push_back(vias.first);
    }
    return imperio;
}

void DFS(mapaEstaciones &m, int v, int p, vector<int> &nodosVisitados, vector<list<int>> &arbolConHijos, vector<int> &nodosConBackedgesQueLlegan, vector<int> &nodosConBackedgesQueSalen){
    // Aplico un DFS de 3 estados como vimos en la práctica que es necesario
    nodosVisitados[v] = AMARILLO;
    for(int w : m[v]){
        if(nodosVisitados[w] == VERDE){
            arbolConHijos[v].push_back(w);
            DFS(m, w, v, nodosVisitados, arbolConHijos,nodosConBackedgesQueLlegan,nodosConBackedgesQueSalen);
        } else if(w != p && nodosVisitados[w] == ROJO) {
            nodosConBackedgesQueLlegan[w]++;
            nodosConBackedgesQueSalen[v]++;
        }
    }
    nodosVisitados[v] = ROJO;
}

int cubren(int v, int p, vector<list<int>> &arbolConHijos, vector<int> &nodosConBackedgesQueLlegan, vector<int> &nodosConBackedgesQueSalen, vector<int> &cantBackedgesQueCubren) {
    // Ahora que tengo el DFS calculo qué nodos tienen aristas puente con el algoritmo de la práctica
    if (cantBackedgesQueCubren[v] != -1) {
        return cantBackedgesQueCubren[v];
    }
    int res = 0;
    res += nodosConBackedgesQueLlegan[v];
    res -= nodosConBackedgesQueSalen[v];
    for (int hijo: arbolConHijos[v]) {
        if (hijo != p) {
            res += cubren(hijo, v,arbolConHijos,nodosConBackedgesQueLlegan,nodosConBackedgesQueSalen,cantBackedgesQueCubren);
        }
    }
    cantBackedgesQueCubren[v] = res;
    return res;
}

vector<pair<int, int>> estacionesABombardear(listaDeEstaciones &l, int n, int m){
    // 1. Paso de lista de aristas a lista de adyacencias
    mapaEstaciones ImperioVIM = TransformarAMapa(l, n);

    // 2. Desmarco todas las estructuras auxiliares
    int cantABombardear = m;

    vector<int> nodosVisitados(n,VERDE); // Diccionario de qué nodos han sido visitados

    vector<list<int>> arbolConHijos(n,list<int>(0)); // Árbol T

    vector<int> nodosConBackedgesQueLlegan(n,0);
    vector<int> nodosConBackedgesQueSalen(n,0);

    vector<int> cantBackedgesQueCubren(n,-1); // Diccionario que nos dice la cantidad de backedges que cubren cada nodo

    vector<vector<int>> pigeonValues(12,vector<int>(0));
    /*
     * Este vector de vectores lo vamos a usar para hacer un bucket sort. Esto es así porque según enunciado tenemos que como mucho una estación
     * de tren esta conectada a otras 10, por lo tanto, el pigeon value máximo que podríamos tener es de 11. Como están acotado el pigeon value
     * puedo ordenar estaciones de mayor a menor en tiempo lineal y con un algoritmo estable. Ahora que expliquemos como seleccionamos las estaciones
     * vamos a explicar la importancia de la estabilidad.
     */

    vector<pair<int,int>> aBombardear(m);
    // Guardamos que estaciones es conveniente bombardean según órden de pigeon value

    // 3. Hago DFS, obtengo el árbol y los diccionarios nodosConBackedgesQueLlegan y nodosConBackedgesQueSalen completos
    DFS(ImperioVIM,0,-1, nodosVisitados, arbolConHijos,nodosConBackedgesQueLlegan,nodosConBackedgesQueSalen); //Genero mi árbol, como mi grafo es conexo puedo hacer DFS para el nodo inicial solamente

    // 4. Una vez hecho el DFS necesito saber qué nodos quedan cubiertos por backedges. Todos esos nodos van a tener pigeon value = 1. Para conocerlos uso el algoritmo de la práctica.
    cubren(0,-1, arbolConHijos,nodosConBackedgesQueLlegan,nodosConBackedgesQueSalen,cantBackedgesQueCubren);

    //5. Calculamos el pigeon value de las estaciones
    /*
     * El pigeon value de las estaciones va a depender del vértice en el árbol
     * Si es una hoja, el pigeon value es 1.
     * Si es la raíz, el pigeon value es la cantidad de hijos de la raíz.
     * Si es un punto de articulación el pigeon value es la cantidad de componentes conexas que se generan al removerlo.
     * Si no es punto de articulación el pigeon value es 1.
     *
     * Voy a ir recorriendo las estaciones en orden creciente (de 0 a n) y guardándolas en el vector de pigeon value
     * donde dijimos que íbamos a hacer el bucket sort. De esta forma ya las tenemos ordenadas en orden creciente a las estaciones
     * solo falta ordenarlas según pigeon value.
     */
    for(int i = 0; i < cantBackedgesQueCubren.size(); i++){
        // Si es una hoja o es un nodo cubierto por una backedge
        if((cantBackedgesQueCubren[i] != 0 && arbolConHijos[i].size() > 0) || arbolConHijos[i].size() == 0){
            if(arbolConHijos[i].size() == 0) { // Caso hoja
                pigeonValues[1].push_back(i);
            } else { // Caso cubierto, me tengo que fijar cuántas aristas de padre a hijos son puente o dicho de otra forma cuántos hijos no están cubiertos por alguna backedge
                int pigeonValue = 0;
                for(int hijo : arbolConHijos[i]){
                    if(cantBackedgesQueCubren[hijo] == 0){
                        pigeonValue++;
                    }
                }
                pigeonValues[pigeonValue + 1].push_back(i);
            }
        } else {
            if(i == 0){ // La raiz tiene un pigeonValue equivalente al de sus hijos
                pigeonValues[arbolConHijos[i].size()].push_back(i);
            } else { // Caso su arista padre es puente
                pigeonValues[1 + arbolConHijos[i].size()].push_back(i);
            }
        }
    }

    // 6. Elijo las mejores estaciones para bombardear
    /*
     * Para hacer esto y ordenarlas por valor de pigeon value decreciente simplemente recorro el vector de pigeon values
     * de atras para adelante y como ya estaban ordenadas las estaciones de forma creciente obtengo el output correcto.
     */
    for(int i = pigeonValues.size() - 1; i > -1 && cantABombardear != 0; i--){
        for(int j = 0; j < pigeonValues[i].size() && cantABombardear != 0; j++){
            aBombardear[m-cantABombardear] = make_pair(pigeonValues[i][j], i);
            cantABombardear--;
        }
    }
    return aBombardear;

}

int main() {
    int estacionesDeTren = -1; // Cantidad de vértices de mi grafo
    int cantEstacionesABombardear = -1;
    while(cin >> estacionesDeTren >> cantEstacionesABombardear){ // Levanto mis datos hasta quedarme sin inputs
        if(estacionesDeTren != 0 && cantEstacionesABombardear != 0){
            listaDeEstaciones ListaVias; // Inicializo mi grafo como lista de aristas
            estaciones vias;
            int estacion1 = 0;
            int estacion2 = 0;
            while(estacion1 != -1 && estacion2 != -1){
                cin>> estacion1 >> estacion2;
                if (estacion1 != -1 && estacion2 != -1){
                    vias = make_pair(estacion1, estacion2);
                    ListaVias.push_back(vias);
                }
            }
            vector<pair<int,int>> estacionesAEliminar = estacionesABombardear(ListaVias, estacionesDeTren, cantEstacionesABombardear);
            for(int i = 0; i < estacionesAEliminar.size(); i++){
                cout << estacionesAEliminar[i].first << " " << estacionesAEliminar[i].second <<endl;
            }
            cout << "\n";
        }
    }
    return 0;
}
