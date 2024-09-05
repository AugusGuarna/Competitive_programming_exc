#include <iostream>
#include <vector>
#include <tuple>
#include <math.h>


using namespace std;
using Geonosis = vector<vector<int>>;
const double infinity = pow(10,5);

/*
 * Vamos a modelar el problema con un digrafo pesado G tal que
 * V(G) = {torres de Geonosis}
 * E(G) = {conexiones entre las torres de Geonosis}
 * c(t_i, t_j) = costo de enviar un mensaje desde t_i a t_j (t_i y t_j son torres de Geonosis)
 * donde c: E(G) --> R >0
 *
 * Como la energía necesitada para destruir una torre se define como el enunciado vamos a estar frente a un problema de APSP.
 * Para resolver el problema vamos a aprovechar el algoritmo de Dantzig que en cada iteración resuelve el problema del APSP
 * para un conjunto k de vértices.
 * Aprovechamos este algoritmo porque luego de destruir una torre tenemos que volver a resolver el problema del APSP entonces si en realidad
 * ordenamos las torres en el orden inverso del orden de destrucción y hacemos Dantzig tenemos el problema resuelto.
 * Luego de destruir la primera torre de acuerdo al orden de destrucción tendremos que calcular el APSP sin esa torre. Si repetimos esto hasta las últimas
 * dos torres vemos que nos quedamos con el APSP para esos dos. Entonces si hacemos Dantzig con el orden de vértices invertido vamos a obtener
 * las soluciones en el orden inverso.
 * Entonces solo resta sumar esas matrices que va calculando Dantzig
 *
 * La implementación de Dantzig es la de la teórica de la noche con la simplificación de que al tener todos costos de aristas positivos
 * no tenemos que detectar ningún ciclo negativo.
 */

long long algoritmoDantzig(Geonosis &mapaImperialRebelion){
    long long energiaNecesariaParaDestruir = 0;
    int tamanoGeonosis = mapaImperialRebelion.size();
    for(int k = 0; k < tamanoGeonosis - 1; k++){
        for(int i = 0; i <= k; i++){
            int minimo1 = infinity;
            int minimo2 = infinity;
            for(int j = 0; j <= k; j++){
                 if(minimo1 > mapaImperialRebelion[i][j] + mapaImperialRebelion[j][k+1]){
                     minimo1 = mapaImperialRebelion[i][j] + mapaImperialRebelion[j][k+1];
                 }
                 if(minimo2 >  mapaImperialRebelion[k+1][j] + mapaImperialRebelion[j][i]){
                     minimo2 = mapaImperialRebelion[k+1][j] + mapaImperialRebelion[j][i];
                 }
            }
            mapaImperialRebelion[i][k+1] = minimo1;
            mapaImperialRebelion[k+1][i] = minimo2;
        }
        for(int i = 0; i <= k; i++){
            for(int j = 0; j <= k; j++){
                mapaImperialRebelion[i][j] = min(mapaImperialRebelion[i][j], mapaImperialRebelion[i][k+1] + mapaImperialRebelion[k+1][j]);
            }
        }
        for(int i = 0; i <= k+1; i++){
            for(int j = 0; j <=k+1; j++){
                energiaNecesariaParaDestruir += mapaImperialRebelion[i][j];
            }
        }
    }
    return energiaNecesariaParaDestruir;
}


long long rogueTwo(Geonosis &mapaImperial, vector<int> &ordenDeDestruccion){
    /*
     * 1. Dada la matriz de costos, vamos a querer ir aplicando el algoritmo de Dantzig para calcular la distancia todos a todos
     * Sin embargo, el orden en el que nos va a venir dada la matriz no me va a servir para calcular Dantzig, tenemos que hacer un reordenamiento
     * de las filas y las columnas para poder aplicar Dantzig más comodamente sin modificar absolutamente nada.
     * Para eso vamos a reordenarla en función del inverso del orden de destrucción.
     */
    int tamanoGeonosis = ordenDeDestruccion.size();
    Geonosis mapaModificado(tamanoGeonosis, vector<int>(tamanoGeonosis, 0));
    for(int i = tamanoGeonosis - 1; i > -1; i--){
        for(int j = tamanoGeonosis - 1; j > -1 ; j--){
            if(i != j){
                mapaModificado[(tamanoGeonosis - 1) - i][(tamanoGeonosis - 1) - j] = mapaImperial[ordenDeDestruccion[i]][ordenDeDestruccion[j]];
            }
        }
    }

    /*
     * 2. Sobre este mapa calculo Dantzig y en cada iteracion voy sumando los valores que acumula mi matriz
     */
    long long energiaNecesariaParaDestruirADarthVader = algoritmoDantzig(mapaModificado);

    return energiaNecesariaParaDestruirADarthVader;
}

int main() {
    // Levanto los planes malvados del imperio galáctico
    int cantTestCases = 0;
    cin >> cantTestCases;
    for(int i = 0; i < cantTestCases; i++){
        int cantTorresImperio = 0;
        cin >> cantTorresImperio;
        Geonosis mapaGeonosis(cantTorresImperio, vector<int>(cantTorresImperio, 0));
        for(int j = 0; j < cantTorresImperio; j++){
            for(int h = 0; h < cantTorresImperio; h++){
                int energiaImperial = 0;
                cin >> energiaImperial;
                mapaGeonosis[j][h] = energiaImperial;
            }
        }
        vector<int> ordenRebelde(cantTorresImperio);
        for(int k = 0; k < cantTorresImperio; k++){
            int torreADestruir = 0;
            cin >> torreADestruir;
            ordenRebelde[k] = torreADestruir;
        }
        long long energiaRogueTwo = rogueTwo(mapaGeonosis, ordenRebelde);
        cout << energiaRogueTwo << endl;
    }

    return 0;
}
