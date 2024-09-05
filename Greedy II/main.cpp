#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;
/*
 * Vamos a resolver el problema mediante una estrategia greedy pero antes de hablar de la misma hablemos de la cantidad y tipo de errores.
 * Para lograr que dos strings sean equivalentes como mínimo necesito hacer 2 swaps entre ellas, por lo tanto,
 * si tengo una cantidad impar de errores entonces el problema no va a tener solución alguna.
 * No obstante, si tengo una cantidad par de errores los puedo diferenciar en 4 tipos:
 *
 * a)
 * a ... b
 * b ... a
 *
 * b)
 * b ... a
 * a ... b
 *
 * c)
 * a ... a
 * b ... b
 *
 * d)
 * b ... b
 * a ... a
 *
 * Entonces erroresTotales = erroresDeA + erroresDeB
 *
 * Con estos tipos de errores en mente vamos a enunciar la estrategia greedy:
 * Cuando la cantidad de caracteres 'a' y 'b' puestos en lugares equivocados sean mayores a 1, entonces siempre
 * voy a tener errores del estilo del caso c) y el caso d). Estos errores se solucionan siempre de la misma manera:
 * supongamos que tenemos dos errores de caracteres 'a' mal ubicados en la posición i y j con i < j de la string de arriba. Para
 * arreglar el error simplemente hago un swap(stringAbajo[i], stringArriba[j]). Notar que el caso d) es análogo solo que tomamos
 * la string de abajo con los errores de a.
 * Si la cantidad de caracteres 'a' y 'b' puestos en lugares equivocados eran par con esto soluciono el problema.
 * Si era impar entonces hago lo explicado recién hasta quedarme con un error de 'a' mal ubicada y con un error de 'b' mal ubicada.
 * En este caso estaríamos en los casos a) o b). Ambos se resuelven iguales:
 * supongamos que tenemos los errores en las posiciones i y j con i < j. Para arreglarlo basta con hacer dos swaps
 * swap(stringArriba[j], stringAbajo[j]) y luego swap(stringArriba[j], stringAbajo[i]). Es decir, lo convierto en un error del tipo
 * c) o d) y lo arreglo como habíamos dicho que los arreglábamos.
 *
 * De esta forma, para encontrar una solución basta con recorrer el vector una sola vez y guardarme las posiciones donde hay errores
 * y aplicar cada caso.
 * Si tenía cantidades impares de errores como me queda simplemente un ultimo tipo de error por resolver lo hago a la salida del ciclo con el
 * que vaya a recorrer.
 */

vector<pair<int, int>> swaps;

int monoCarpSwap(string &s, string &t){
    int cantErrores = 0;
    int cantAs = 0;
    int cantBs = 0;
    int res = -1;
    swaps = {};
    // Cuento la cantidad de errores
    for(int i = 0; i < s.size(); i++){
        if(s[i] == 'a' && t[i] == 'b' || s[i] == 'b' && t[i] == 'a'){
            cantErrores++;
            if (s[i] == 'a'){
                cantAs++;
            } else {
                cantBs++;
            }
        }
    }
    // Si la cantidad de errores es par busco una de las soluciones existentes con la estrategia greedy
    if (cantErrores % 2 == 0) {
        res++;
        int primeraApA = -1;
        int ultApA = -1;
        int primeraApB = -1;
        int ultApB = -1;
        for(int i = 0; i < s.size(); i++){
            // Tipo de error c)
            if (s[i] == 'a' && t[i] == 'b') {
                if (primeraApA == -1) {
                    primeraApA = i;
                } else {
                    ultApA = i;
                    s[primeraApA] = 'b';
                    t[ultApA] = 'a';
                    cantAs = cantAs - 2;
                    res++;
                    swaps.push_back(make_pair(primeraApA + 1, ultApA + 1));
                    primeraApA = -1;
                    ultApA = -1;
                }
            }
            // Tipo de error b)
            if (s[i] == 'b' && t[i] == 'a') {
                if (primeraApB == -1) {
                    primeraApB = i;
                } else {

                    ultApB = i;
                    s[primeraApB] = 'a';
                    t[ultApB] = 'b';
                    cantBs = cantBs - 2;
                    res++;
                    swaps.push_back(make_pair(primeraApB + 1, ultApB + 1));
                    primeraApB = -1;
                    ultApB = -1;
                }
            }
        }
        // Tipo de error a) o b)
        if(primeraApB != -1 && primeraApA != -1) {
            res = res + 2;
            cantAs--;
            cantBs--;
            if(primeraApB < primeraApA){
                s[primeraApA] = 'b';
                t[primeraApA] = 'a';
                int temporal = primeraApB;
                primeraApB = primeraApA;
                primeraApA = temporal;
                s[primeraApA] = 'a';
                t[primeraApB] = 'b';
            } else{
                s[primeraApB] = 'a';
                t[primeraApB] = 'b';
                int temporal = primeraApA;
                primeraApA = primeraApB;
                primeraApB = temporal;
                s[primeraApB] = 'b';
                t[primeraApA] = 'a';
            }
            swaps.push_back(make_pair(primeraApA + 1, primeraApA + 1));
            swaps.push_back(make_pair(primeraApB + 1, primeraApA + 1));
        }
    }
    return res;
}

int main() {
    int tamanoStrings = 0;
    cin >> tamanoStrings;

    string s;
    string t;
    cin >> s >> t;

    int minSwaps = monoCarpSwap(s,t);
    cout << minSwaps << endl;
    if (minSwaps > 0) {
        for (int i = 0; i < swaps.size(); i++) {
            cout << swaps[i].first << " " << swaps[i].second << endl;
        }
    }

    return 0;
}
