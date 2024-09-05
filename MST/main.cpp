#include <iostream>
#include <vector>

using namespace std;


/* Se uso la implementación de Prim que dio el Dr. Soulignac en la teórica
 * Pseudo-código
 *
 * Prim(G: grafo, c: funcion de costos, r: vértice inicial)
 * 1. Sea T vector con T[r] = r y T[w] = bottom para w perteneciente a V(G)\{r} // árbol
 * 2. Sea C un diccionario de costos/ C[w] = c(rw) si w pertenece a N(r) y C[w] = infinito si no
 * 3. Sea P un diccionario/ P[w] = r para w perteneciente a N(r) // diccionario de padres
 * 4. Para i = 1, ... , n - 1
 * 5.   Sea v = P[w] para el vértice 2 con costo mínimo en C
 * 6.   Poner T[w] = v y eliminar w de C
 * 7.   Para cada z en N(w) con T[z] = bottom
 * 8.   Si c(wz) < C[z] decrementar C[z] a c(wz) y poner P[z] = w
 */

/*
 * Vamos a modelar el problema usando grafos. Vamos a considerar nuestro grafo pesado G tal que
 * 1. V(G) = {llaves del test case}
 * 2. E(G) = {vw / v, w pertenecen a V(G) y representan la posibilidad de movernos de una clave a otra}
 * 3. C : E(G) -> R / C(vw) = costo necesario para pasar de v a w o viceversa (total es el mismo)
 *
 * Como de cada clave puedo pasar a cualquier otra clave, cada vértice va a tener #V(G) - 1 aristas que salgan de él.
 * Por lo tanto, vamos a tener un grafo completo y, en particular, denso.
 * Como vamos a querer un árbol tal que tenga los mismos vértices que G, un subconjunto de sus aristas y
 * cuyas aristas sumen la mínima cantidad de pasos posibles, vamos a querer un AGM de G.
 * Lo que nos permite plantear un AGM no solo es el modelado sino también el hecho de que podemos volver a una clave
 * que ya descubrimos con costo 0.
 * Como G es denso vamos a usar la versión de Prim cuadrática donde nuestro vértice inicial puede ser cualquiera.
 * Al usar Prim cuadrático, podemos representar al grafo como matriz de adyacencias
 * No obstante, además de hacer el AGM sobre el conjunto de claves que me pasan tengo que también calcular cuál es el mínimo costo de pasar
 * de la clave 0000 a cualquier otra de las claves. Este sería el costo que hay que pagar para empezar a probar claves
 */

int calcularPasoDeUnaKeyAOtra(int key1, int key2){
    /*
   * El costo de pasar a una key a otra es equivalente a la suma de los costos de cambiar cada dígito
   * Si ninguno de los 2 es 0 entonces:
   * Cambiar cada dígito tiene un costo igual al min{entre girar para arriba, girar para abajo}
   * Esto es así porque supongamos el caso de pasar del 1 al 9
   * Podríamos ir girando para arriba, es decir, de 1 a 2, de 2 a 3 y así. Esto cuesta 8.
   * Sin embargo, si giramos para abajo, es decir, de 1 a 0, de 0 a 9. Esto tiene un costo de 2.
   * No obstante, para calcular cada uno vamos a considerar 2 formas de girar:
   * Paso por 0, no paso por 0.
   * Cuando no paso por 0 lo que hago es girar de x a y o de y a x sin pasar por el cero.
   * Cuando paso por el 0 puedo ir de x a y pasando por el 0.
   * Para calcular el costo de "no paso por 0" basta con calcular el módulo de la diferencia.
   * Para calcular el costo de "paso por 0":
   * Supongamos que costo1 >= costo 2 sin perdida de generalidad
   * Podríamos de forma iterativa ir sumando 1 a costo1 hasta que pasemos el 9 y de ahí fueramos al 0 y comenzáramos de vuelta
   * hasta llegar al más pequeño. Esto es lo mismo que hacer (10-costo1) + costo2. De la misma forma podríamos ir de costo2 a 0
   * y de 0 a 9 y de ahí ir restando hasta llegar al más grande.
   *
   * Si alguno es 0:
   * Si costo 1 es 0 y costo2 <= 5, el costo es costo2
   * Si costo 1 es 0 y costo2 > 5, el costo es 10 - costo2
   */
    int sumaCosto = 0;
    // Para sumar el módulo de la diferencia dígito a dígito vamos a tomar su módulo 10 y luego dividirlo por 10.
    for(int h = 0; h < 4; h++){
        if (key1 % 10 != 0 && key2 % 10 != 0){
            if (key1 % 10 > key2 % 10){
                sumaCosto += min(abs((key1 % 10) - (key2 % 10)), (10 - (key1 % 10)) + (key2 % 10));
            } else {
                sumaCosto += min(abs((key1 % 10) - (key2 % 10)), (10 - (key2 % 10)) + (key1 % 10));
            }
        } else {
            sumaCosto += min(key1 % 10 + key2 % 10, 10 -(key1 % 10 + key2 % 10));
        }
        key1 = key1/10;
        key2 = key2/10;
    }
    return sumaCosto;
}

vector<vector<int>> construirMatrizDeAdyacenciasYCostos(vector<int> &v){
    vector<vector<int>> M(v.size(),vector<int>(v.size(), -1));
    /*
     * Declaro la matriz y empiezo a llenarla con los costos
     * No tiene sentido calcular primero la primera fila, luego la segunda y así ya que cuando calculamos la primera fila también la primera columna,
     * por lo tanto, podemos ir tomando submatrices y acelerar el proceso
     */
    for(int i = 0; i < M.size(); i++){
        for(int j = i; j < M.size(); j++){
            if(i != j){
                int costo = calcularPasoDeUnaKeyAOtra(v[i], v[j]);
                M[i][j] = costo;
                M[j][i] = costo;
            }
        }
    }
    return M;
};
// Implemento el algoritmo de Prim como lo dio el Dr. Soulignac en la teórica
vector<int> algoritmoPrimCuadratico(vector<vector<int>> M, int v){
    /*
     * 1. Inicializamos la estructura donde guardaremos el AGM, armamos un diccionario de costos implementado en un vector
     * y hacemos un diccionario de padres
     */
    vector<int> arbol(M.size(), -1);
    arbol[0] = v;
    vector<int> costos = M[0];
    vector<int> padres(M.size(), v);
    /*
     * 2. Iteramos sobre todos los nodos y hacemos lo mismo que haría el algoritmo de Prim implementado sobre una cola
     * de prioridad pero en los vectores
     */
    for(int i = 0; i < M.size() - 1; i++){
        // Busco el w con costo mínimo en mi diccionario y su padre
        int costoMin = 9*4; // Como vamos a querer buscar el menor valor en el vector de costos inicializo el valor en el máximo costo posible que puede tener una arista, pasar de 0000 a 9999
        int w = 0;
        for(int j = 0; j < M.size(); j++){
            if(costos[j] < costoMin && costos[j] != -1){
                costoMin = costos[j];
                w = j;
            }
        }
        // Guardo la info en mi árbol
        int padre = padres[w];
        arbol[w] = padre;

        // Tengo que eliminar costos[w], lo asigno como -1
        costos[w] = -1;

        // Para el vértice que acabo de agregar a mi árbol tengo que calcular las aristas candidatas y ver si su costo es menor que el de las que ya tenía
        for(int h = 0; h < M.size(); h++){
            if(arbol[h] == -1){
                if(M[w][h] != -1 && M[w][h] < costos[h]) {
                    costos[h] = M[w][h];
                    padres[h] = w;
                }
            }
        }


    }
    return arbol;
}

int cantMinPasosParaDesbloquear(vector<int> &v){
    /*
     * 1. Tengo que modelar el problema con un grafo, vamos a hacer la matriz y calcular los pesos de las aristas.
     * Como voy a tener un grafo completo, en la misma matriz de adyacencias puedo ya directamente poner lo que me cuesta pasar de una arista a otra
     * y así tengo una matriz de costos donde si dos vértices son adyacentes tengo el costo de pasar de uno a otro y sino tengo un -1.
     */
    vector<vector<int>> matrizDeAdyacenciasYCostos = construirMatrizDeAdyacenciasYCostos(v);
    /*
     * 2. Ahora que tengo el grafo y sus costos puedo aplicar el algoritmo de Prim para obtener su AGM
     */
    vector<int> AGM = algoritmoPrimCuadratico(matrizDeAdyacenciasYCostos, 0);
    /*
     * 3. Ahora que tengo el AGM queda sumar el costo de sus aristas
     */
    int costoTotal = 0;
    for(int i = 1; i < AGM.size() ; i++){
        costoTotal += matrizDeAdyacenciasYCostos[AGM[i]][i];
    }
    // Ahora queda calcular la distancia de cada key a la key 0000 y quedarme con la menor distancia
    int min = 9*4;
    for(int i = 0; i < v.size(); i++){
        int minLocal = calcularPasoDeUnaKeyAOtra(0000, v[i]);
        if(minLocal < min){
            min = minLocal;
        }
    }
    costoTotal += min;
    return costoTotal;
}

int main() {
    int cantTestCases = 0;
    cin >> cantTestCases; // Levanto la cantidad de test que me van a pasar
    for(int i = 0; i < cantTestCases; i++){
        // Para cada test levanto la cantidad de keys del mismo
        int cantLlaves = 0;
        cin >> cantLlaves;
        /*
         * Para que luego sea más fácil trabajar con los números de las keys vamos a declarar un vector que va a servir de
         * diccionario. A cada key le vamos a asignar un número del 1 a cantLlaves
         */
        vector<int> DiccKeys(cantLlaves);

        for(int j = 0; j < cantLlaves; j++){
            int key = 0;
            cin >> key;
            DiccKeys[j] = key;
        }
        int cantMinPasos = cantMinPasosParaDesbloquear(DiccKeys);
        cout << cantMinPasos << endl;
    }

    return 0;
}
