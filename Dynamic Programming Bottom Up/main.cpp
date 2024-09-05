#include <iostream>
#include <vector>

using namespace std;



int mayorCantidadDeBellotas(vector<vector<int>> &posBellotas, int h, int f){
    /*
     * En mi estructura voy a necesitar guardar las mejores bellotas por nivel para evitar gastos
     * innecesarios de memoria. Usar una matriz de (altura de los árboles) x (cantidad de árboles)
     * resulta en un gasto innecesario de memoria ya que al final de cuentas, luego calculo el mejor para
     * cada posición y luego saco el mejor de la fila. Para eso directamente me guardo el mejor de la fila
     */

    vector<int> mejoresBellotas(h+1);

    /*
     * Por cada posición que yo analice, como voy a recorrer la matriz de datos de abajo hacia arriba,
     * si yo quiero el número de bellotas por posición tengo que quedarme con el máximo entre la posición
     * anterior y las posiciones de las que pude haber saltado.
     * El máximo por fila es el máximo de todas las posiciones de la fila.
     * Ahora, la gracia de la estructura es que pueda aprovecharla. De analizar el caso de la matriz
     * vemos que el máximo de cada fila es igual al máximo de la fila anterior más el número de bellotas de una
     * posición.
     */

    // La mejor cantidad de bellotas para la altura cero es cero
    mejoresBellotas[0] = 0;

    /*
     * Empezamos a calcular los máximos por nivel. Para eso en la primera fila ya teniendo los resultados
     * de la 0.
     */
    for(int j = 1; j < posBellotas.size(); j++){
        mejoresBellotas[j] = 0;
        for(int k = 0; k < posBellotas[0].size(); k++){
            int cantBellotas = 0;
            if(j-f > -1) { // Puedo saltar entre árboles porque la altura me lo permite
                cantBellotas = max(posBellotas[j][k] + posBellotas[j-1][k], mejoresBellotas[j-f] + posBellotas[j][k]);
                /*
                 * La cantidad máxima de bellotas para esa posición va a ser el máximo entre haberme quedado en el árbol
                 * (y seguir escalándolo) y haber saltado de otra posición a esta.
                 * Para calcular el valor que se obtiene de haber saltado de otra posición a la actual vamos a considerar
                 * el máximo de la fila que ya teníamos calculado. Esto implicaría saltar de la posición con más bellotas a
                 * la actual.
                 * Como en la comparación tomamos el máximo entre seguir en el árbol y saltar de otra posición a la actual,
                 * no ocurre que la ardilla pueda saltar dentro del mismo árbol. Ya que vamos a ir acumulando los valores
                 * de haberse quedado en el árbol.
                 */
            } else {
                cantBellotas = posBellotas[j][k] + posBellotas[j-1][k];
                // Si no puedo saltar, entonces sumo el valor de la fila anterior al de la fila actual
            }
            posBellotas[j][k] = cantBellotas; // Guardo acumulados
            mejoresBellotas[j] = max(cantBellotas, mejoresBellotas[j]);
            /*
             * La mejor cantidad de bellotas para esa fila va a ser el máximo entre el máximo ya calculado y el máximo
             * que calculé para la nueva posición
             */
        }
    }

    return mejoresBellotas[h];
}

int main() {
    int cantidadDatasets = 0;
    cin >> cantidadDatasets; // Levantamos la cantidad de datasets del problema
    vector<vector<int>> posBellotas; // Creamos una matriz para guardar las bellotas, vendría a representar los árboles del bosque con sus bellotas
    for(int i = 0; i < cantidadDatasets; i++){
        int cantidadArboles = 0;
        int alturaArboles = 0;
        int alturaDeDescenso = 0;
        cin >> cantidadArboles >> alturaArboles >> alturaDeDescenso;
        posBellotas = vector<vector<int>>(alturaArboles+1, vector<int>(cantidadArboles, 0)); // Inicializo la matriz con su tamaño
        for(int j = 0; j < cantidadArboles; j++){ // Asigno las bellotas a cada posición
            int cantBellotas = 0;
            cin >> cantBellotas;
            for(int k = 0; k < cantBellotas; k++){
                int hBellota = 0;
                cin >> hBellota;
                posBellotas[hBellota][j] += 1; // Sumamos una por si tenemos más de una bellota por posición
            }
        }
        int maxBellotas = mayorCantidadDeBellotas(posBellotas, alturaArboles, alturaDeDescenso);
        cout << maxBellotas << endl;
    }
    return 0;
}
