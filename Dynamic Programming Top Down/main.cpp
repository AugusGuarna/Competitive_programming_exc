#include <iostream>
#include <vector>



using namespace std;
//Declaro las estructuras de memoización
vector<vector<int>> Mc;
vector<vector<int>> Dc;

/* Función para obtener la subsecuencia creciente más larga
 * Vamos a recorrer la secuencia de edificios desde la posición 0 hasta la n
 */

int creciente(vector<int> &alturas, vector<int> &anchos, int i, int k){
    if (i == alturas.size()){ // Caso base
        return 0;
    } else {
        if (k >= alturas[i]){ // Si la altura de esta posición no es mayor a la que ya agregué no la agrego
            return creciente(alturas, anchos, i+1, k);
        } else {
            // Si es mayor me fijo el máximo entre agregar o no
            if (Mc[i][k] == -1){
                int conEseEdificio =creciente(alturas, anchos, i+1, alturas[i]) + anchos[i];
                int sinEseEdificio = creciente(alturas, anchos, i+1, k);
                Mc[i][k] = max(conEseEdificio,sinEseEdificio);
            }
            return Mc[i][k];
        }
    }
}

/* Función para obtener la subsecuencia decreciente más larga
 * Vamos a recorrer la secuencia de edificios desde la posición n hasta la 0. Si lo hicieramos como
 * en el caso de arriba, para resolver el problema necesitariamos llamar a la función de la siguiente manera
 * decreciente(alturas, anchos, 0, +inf) y ese +inf trae problemas al querer aprovechar mi estructura de memoización
 */

int decreciente(vector<int> &alturas, vector<int> &anchos, int i, int k){
    if (i == -1){ // Caso base
        return 0;
    } else {
        if (k >= alturas[i]){ // Si la altura de esta posición no es mayor a la que ya agregué no la agrego, al ser decreciente necesitamos que la anterior a la agregada sea mayor
            return decreciente(alturas, anchos, i-1, k);
        } else {
            // Si es mayor me fijo el máximo entre agregar o no
            if (Dc[i][k] == -1){
                int conEseEdificio = decreciente(alturas, anchos, i-1, alturas[i]) + anchos[i];
                int sinEseEdificio = decreciente(alturas, anchos, i-1, k);
                Dc[i][k] = max(conEseEdificio, sinEseEdificio);
            }
            return Dc[i][k];
        }
    }
}

int main() {
    int cantTests = 0;
    cin >> cantTests;
    for (int i = 0; i < cantTests; i++) { // Levanto los valores de la consola
        vector<int> alturas = {};
        vector<int> anchos = {};
        int altura = 0;
        int ancho = 0;
        int longitudVectores = 0;
        cin >> longitudVectores;
        for (int j = 0; j < longitudVectores; j++) {
            cin >> altura;
            alturas.push_back(altura);
        }
        for (int j = 0; j < longitudVectores; j++) {
            cin >> ancho;
            anchos.push_back(ancho);
        }
        int maxAlt = 0;
        // Busco cuál es la máxima altura de mi secuencia de edificios para poder inicializar la matriz
        for (int i = 0; i < alturas.size();i++){
            if (alturas[i] > maxAlt){
                maxAlt = alturas[i];
            }
        }

        Mc = vector<vector<int>>(alturas.size(), vector<int>(maxAlt, -1));
        Dc = vector<vector<int>>(alturas.size(), vector<int>(maxAlt, -1));
        int longitudCreciente = creciente(alturas, anchos, 0, 0);
        int longitudDecreciente = decreciente(alturas, anchos, longitudVectores-1, 0);

        //Imprimo resultados
        cout << "Case " << i + 1 << ". ";
        if (longitudCreciente >= longitudDecreciente){
          cout << "Increasing (" << longitudCreciente << "). ";
          cout << "Decreasing (" << longitudDecreciente << ").\n";
        } else {
            cout << "Decreasing (" << longitudDecreciente << "). ";
            cout << "Increasing (" << longitudCreciente << ").\n";
        }

    }
    return 0;
}