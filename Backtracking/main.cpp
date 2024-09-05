#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;
// En las variables globales voy a guardar los mejores resultados
vector<string> mejoresAtacantes = {};
vector<string> mejoresDefensores = {};
int mejorAtScore = 0;
int mejorDefScore = 0;

void definirAlineacion(vector<tuple<string, int, int>> listaJugadores, int i, int atScore, int defScore, vector<string> &atacantes, vector<string> &defensores) {
    if (i == 10) { // Caso base. Corta mi recursión cuando ya viste los 10 jugadores
        if (atScore >= mejorAtScore) {
            if (atScore > mejorAtScore) {
                mejorAtScore = atScore;
                mejorDefScore = defScore;
                mejoresAtacantes = atacantes;
                mejoresDefensores = defensores;
                // Ya ordeno mis defensores y atacantes ahora para dejarlos listos para comparar por órden lexicográfico y para cuando necesite devolverlos
                sort(mejoresAtacantes.begin(), mejoresAtacantes.end());
                sort(mejoresDefensores.begin(), mejoresDefensores.end());
            } else if (defScore >= mejorDefScore) {
                if (defScore > mejorDefScore) {
                    mejorAtScore = atScore;
                    mejorDefScore = defScore;
                    mejoresAtacantes = atacantes;
                    mejoresDefensores = defensores;
                    sort(mejoresAtacantes.begin(), mejoresAtacantes.end());
                    sort(mejoresDefensores.begin(), mejoresDefensores.end());
                } else {
                    vector<string> comparar = atacantes;
                    // Guardo mis atacantes en un nuevo vector para evitar desordenar mis soluciones parciales y arruinar la exploración
                    sort(comparar.begin(), comparar.end());
                    /*
                     * Ordeno el vector a comparar con los mejores atacantes ya que podría ocurrir que tuviera que comparar
                     * un vector {a,a,a,a,z} con otro {b,a,a,a,a}. El vector solución sería el segundo pero por la comparación de órdenes lexicográficos, si no lo ordeno
                     * podría devolver el primero, lo que es erróneo.
                     */
                    bool esMejor = false;
                    for (int i = 0; i < mejoresAtacantes.size(); i++) {
                        if (comparar[i] < mejoresAtacantes[i]) {
                            esMejor = true;
                        }
                    }
                    if (esMejor) {
                        mejoresAtacantes = comparar;
                        mejoresDefensores = defensores;
                        sort(mejoresDefensores.begin(), mejoresDefensores.end());
                    }
                }
            }
        }
    } else {
            /*
             * Podas por factibilidad: mi ataque no puede tener más de 5 jugadores, mi defensa tampoco.
             */
            if (atacantes.size() == 5){
                if(atScore >= mejorAtScore){
                    defensores.push_back(get<0>(listaJugadores[i]));
                    definirAlineacion(listaJugadores, i+1, atScore, defScore + get<2>(listaJugadores[i]), atacantes, defensores);
                    defensores.pop_back();
                }
            } else if (defensores.size() == 5){
                atacantes.push_back(get<0>(listaJugadores[i]));
                definirAlineacion(listaJugadores, i+1, atScore + get<1>(listaJugadores[i]), defScore, atacantes, defensores);
                atacantes.pop_back();
            } else {
                // Si ambos tienen menos de 5 hace la recursión normal
                atacantes.push_back(get<0>(listaJugadores[i]));
                definirAlineacion(listaJugadores, i+1, atScore + get<1>(listaJugadores[i]), defScore, atacantes, defensores);
                atacantes.pop_back();
                defensores.push_back(get<0>(listaJugadores[i]));
                definirAlineacion(listaJugadores, i+1, atScore, defScore + get<2>(listaJugadores[i]), atacantes, defensores);
                defensores.pop_back();
            }

    }
}

int main() {
    int cantTests = 0;
    cin >> cantTests;
    for (int i = 0; i < cantTests; i++) { // Levanto los valores de la consola
        vector<tuple<string, int, int>> listaJugadores = {};
        vector<string> atacantes = {};
        vector<string> defensores = {};
        vector<string> jugadores = {};
        string nombre = "";
        int atRating = 0;
        int defRating = 0;
        for (int j = 0; j < 10; j++) { // Armo la lista de jugadores para los test case
            cin >> nombre >> atRating >> defRating;
            listaJugadores.push_back(make_tuple(nombre, atRating, defRating));
            jugadores.push_back(nombre);
        }
        definirAlineacion(listaJugadores, 0, 0, 0,atacantes, defensores); // Una vez que levante los 10 jugadores del caso de test, corro la función
        //Imprimo resultados
        cout << "Case " << i + 1 << ":\n";
        cout << "(";
        for (int h = 0; h < mejoresAtacantes.size(); h++) {
            if (h < mejoresAtacantes.size() - 1) {
                cout << mejoresAtacantes[h] << ", ";
            } else {
                cout << mejoresAtacantes[h] << ")\n";
            }
        }
        cout << "(";
        for (int h = 0; h < mejoresDefensores.size(); h++) {
            if (h < mejoresDefensores.size() - 1) {
                cout << mejoresDefensores[h] << ", ";
            } else {
                cout << mejoresDefensores[h] << ")\n";
            }
        }
        mejoresAtacantes = {}; // Reinicio el vector que guarda los máximos atacantes para evitar problemas cuando haya más de un caso de test
        mejoresDefensores = {}; // Reinicio el vector que guarda los máximos defensores para evitar problemas cuando haya más de un caso de test
        mejorAtScore = 0; // Reinicio estos valores para evitar problemas
        mejorDefScore = 0;

    }
    return 0;
}