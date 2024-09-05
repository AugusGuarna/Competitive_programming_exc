#include <iostream>
#include <vector>

using namespace std;

long long cantidadTrabajoTransporteVinos(vector<long long> &vinos){
    long long trabajoNecesario = 0; // Acumulo el trabajo necesario que implica mover los vinos
    long long acumVinos = 0; // Acumulo la oferta y demanda que van a pasar a tener los sucesivos vecinos
    for(int i = 0; i < vinos.size(); i++){
        acumVinos += vinos[i]; // Voy sumando ofertas y demandas
        trabajoNecesario += abs(acumVinos); // Es igual a mover los vinos que fui acumulando en módulo, su signo es solo para diferenciar oferta y demanda
    }
    return trabajoNecesario;
}

int main() {
    long long longitudDeLaCiudad = 0;
    long long habitante = 0;
    while(cin >> longitudDeLaCiudad){ // Levanto la cantidad de habitantes de la ciudad de mi test case
        if(longitudDeLaCiudad != 0){ // Me fijo que no haya llegado al final de los test cases
            vector<long long>ciudad(longitudDeLaCiudad);
            for(int i = 0; i < longitudDeLaCiudad; i++){ // Guardo la ciudad
                cin >> habitante;
                ciudad[i] = habitante;
            }
            /*
             * Ahora, mi algoritmo podría ser de fuerza bruta y eso me devolvería el mínimo pero defino
             * la siguiente estrategia golosa que va a resultar en un algoritmo de complejidad O(n).
             *
             * Estrategia golosa: el mínimo trabajo necesario para satisfacer la demanda va a ser equivalente
             * a ir pasando la demanda (o la oferta) de vecino en vecino. Es decir, si el i-ésimo vecino
             * quiere comprar x botellas, le paso esa demanda al (i+1)-ésimo vecino. Si el (i+1)-ésimo vecino
             * quería comprar y botellas, ahora va a querer comprar (x+y). Si quería vender y botellas, ahora
             * va a querer vender (o comprar si  x > y) y-x botellas. Así sucesivamente.
             * Como los números del vector suman 0, mediante esta estrategia cuando haya llegado a la última
             * posición voy a haber satisfecho toda la oferta y demanda (me van a quedar todas las posiciones en 0)
             * y encima voy a tener el mínimo trabajo necesario calculado.
             *
             * Es una estrategia golosa porque para cada posición estoy considerando el óptimo local (el óptimo local
             * sería pagar todas las unidades de trabajo que requieren mover los vinos de mi i-ésimo vecino en
             * orden de satisfacer su oferta o demanda. De esta manera estoy pagando el mínimo necesario para
             * satisfacerlo) esperando que sea el óptimo global.
             * Mediante inducción se podría probar que así se llega al mínimo.
             * También se puede pensar que si tengo un vector de 4 posiciones {3,-2,0,-1} el mínimo (sin aplicar
             * la estrategia golosa) sería igual a satisfacer primero a 2 y después fijarme quién tiene que ser
             * satisfecho y hacerlo con 4. Ese mínimo es el mismo que al que llego con mi estrategia golosa,
             * donde me ahorro recorrer el vector más de una vez.
             */
            long long minimoTrabajo = cantidadTrabajoTransporteVinos(ciudad);
            cout << minimoTrabajo << endl;
        }
    }
    return 0;
}
