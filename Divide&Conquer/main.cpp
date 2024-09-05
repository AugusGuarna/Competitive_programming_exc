#include <iostream>
#include <string>

using namespace std;


bool sonEquivalentes(string &s1, string &s2, int L1, int R1, int L2, int R2){
    if ((R1-L1) % 2 == 0 || R1 == L1){ // Conquer. Si las strings tienen largos impares, para que sean equivalentes tienen que ser iguales. Si no, el otro caso base es que las strings tengan largo 1
        if (R1 == L1){ // si tienen largo uno me fijo que sean la misma
            return s1[L1] == s2[L2];
        } else {
            return s1.substr(L1,(R1-L1)+1) == s2.substr(L2,(R2-L2)+1); // si son impares
        }
    } else { // Divide y combine en simultáneo. Divido las strings como el problema dice y combino los resultados mediante una concatenación de ORs.
        // Hacemos el divide con índices para que sea en O(1) y no tener que pagar el costo de dividir y copiar.
        return sonEquivalentes(s1,s2,L1, (R1+L1)/2, L2,(R2+L2)/2) && sonEquivalentes(s1,s2,(R1+L1)/2 + 1, R1, (R2+L2)/2 + 1,R2) ||
        sonEquivalentes(s1,s2,L1, (R1+L1)/2, (R2+L2)/2 + 1,R2) && sonEquivalentes(s1,s2,(R1+L1)/2 + 1, R1, L2 ,(R2+L2)/2);
    }
}


int main() {
    string primeraPalabra;
    string segundaPalabra;
    cin >> primeraPalabra >> segundaPalabra;
    bool res = sonEquivalentes(primeraPalabra, segundaPalabra,0,primeraPalabra.size()-1,0,segundaPalabra.size()-1);
    if (res){
        cout << "YES";
    } else {
        cout << "NO";
    }

    return 0;
}
