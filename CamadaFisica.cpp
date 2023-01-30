#include "CamadaFisica.hpp"

// Vari�veis globais para definir o tipo de codifica��o e decodifica��o utilizadas
volatile int tipoDeCondificacao = 2;
volatile int tipoDeDecodificacao = 2;

/*
  Fun��o que solicita uma mensagem ao usu�rio e a envia para a camada de aplica��o transmissora
 */
void AplicacaoTransmissora(void){
    string mensagem;
    cout << "Entre com a mensagem" << endl;
    cin >> mensagem;
    CamadaDeAplicacaoTransmissora(mensagem);
}

/*
 * Fun��o que transforma a mensagem em um vetor de bits
 * @param mensagem A mensagem a ser enviada
 */
void CamadaDeAplicacaoTransmissora (string mensagem) {
    vector<int> quadro;
    bitset<8> byte;
    int aux;
    for (char const &c: mensagem) {
        byte = bitset<8>(c);
        for (int i = 0; i < 8; i++) {
            aux = (int)((bitset<1>(byte[i])).to_ulong());
            quadro.insert(quadro.begin() + i, aux);
        }
    }
    CamadaFisicaTransmissora(quadro);
}

/*
 * Fun��o que seleciona o tipo de codifica��o a ser utilizada e envia o quadro de bits para o meio de comunica��o
 * @param quadro O vetor de bits a ser enviado
 */
void CamadaFisicaTransmissora (vector<int> quadro) {
    vector<int> fluxoBrutoDeBits;

    switch (tipoDeCondificacao) {
        case 0:
            fluxoBrutoDeBits = CamadaFisicaTransmissoraCodificacaoBinaria(quadro);
            break;
        case 1:
            fluxoBrutoDeBits = CamadaFisicaTransmissoraCodificacaoManchester(quadro);
            break;
        case 2:
            fluxoBrutoDeBits = CamadaFisicaTransmissoraCodificacaoBipolar(quadro);
            break;
    }
    MeioDeComunicacao(fluxoBrutoDeBits);
}

/*
 * Fun��o que codifica os bits com a codifica��o bin�ria
 * Transforma os bits positivos (1) em 5v e os bits zeros (0) em 0v
 * @param quadro O vetor de bits a ser codificado
 * @return O vetor de bits codificado
 */
vector<int> CamadaFisicaTransmissoraCodificacaoBinaria (vector<int> quadro) {
    int size_vector = (int) quadro.size();
    for (int inner = 0; inner < size_vector; inner++) {
        if (quadro[inner] == 0){
            quadro.at(inner) = 0;
        }else{
            quadro.at(inner) = 5;
        }
    }
    return quadro;
}

/*
Fun��o que codifica o quadro com a t�cnica Manchester.
A codifica��o � feita aplicando XOR com 1 na amplitude recebida e depois com 0 no resultado.
@param vector<int> quadro - Vetor com o quadro a ser codificado.
@return vector<int> - Vetor com o quadro codificado.
*/
vector<int> CamadaFisicaTransmissoraCodificacaoManchester (vector<int> quadro) {
    int size_vector = (int) quadro.size();
    for (int inner = 0; inner < size_vector; inner++) {
        quadro.at(inner) = quadro[inner] ^ 0;
        quadro.at(inner) = quadro[inner] ^ 1;
    }
    return quadro;
}

/*
Fun��o que codifica o quadro com a t�cnica Bipolar.
A codifica��o � feita alternando entre 1 e -1 quando o bit � igual a 1.
@param vector<int> quadro - Vetor com o quadro a ser codificado.
@return vector<int> - Vetor com o quadro codificado.
*/
vector<int> CamadaFisicaTransmissoraCodificacaoBipolar (vector<int> quadro) {
    int size_vector = (int) quadro.size();
    bool alterna = true;
    for (int inner = 0; inner < size_vector; inner++) {
        if (quadro[inner] == 1){
            if (alterna){
                quadro.at(inner) = 1;
            }else{
                quadro.at(inner) = -1;
            }
            alterna = !alterna;
        }
    }
    return quadro;
}

/*
Fun��o que envia o quadro pelo meio de comunica��o.
O quadro � replicado e enviado para a camada f�sica receptora.
@param vector<int> fluxoBrutoDeBits - Vetor com o quadro a ser enviado.
*/
void MeioDeComunicacao (vector<int> fluxoBrutoDeBits) {
    vector<int> fluxoBrutoDeBitsPontoA, fluxoBrutoDeBitsPontoB;

    fluxoBrutoDeBitsPontoA = fluxoBrutoDeBits;
    int size = fluxoBrutoDeBitsPontoA.size();

    for (int i = 0; i < size; i++ ) {
        fluxoBrutoDeBitsPontoB.insert(fluxoBrutoDeBitsPontoB.begin() + i, fluxoBrutoDeBitsPontoA[i]);
    }

    CamadaFisicaReceptora(fluxoBrutoDeBitsPontoB);
}

/*
@brief Fun��o respons�vel por decodificar o quadro de acordo com o tipo de codifica��o.
A fun��o realiza a decodifica��o do quadro de acordo com o tipo de codifica��o escolhido.
A decodifica��o pode ser bin�ria, Manchester ou bipolar.
Em seguida, a fun��o chama a Camada de Aplica��o Receptora para processar o fluxo bruto de bits.
@param quadro - vetor de inteiros representando o quadro a ser decodificado.
*/
void CamadaFisicaReceptora (vector<int> quadro) {
    vector<int> fluxoBrutoDeBits;
    switch (tipoDeDecodificacao) {
        case 0:
            fluxoBrutoDeBits = CamadaFisicaReceptoraCodificacaoBinaria(quadro);
            break;
        case 1:
            fluxoBrutoDeBits = CamadaFisicaReceptoraCodificacaoManchester(quadro);
            break;
        case 2:
            fluxoBrutoDeBits = CamadaFisicaReceptoraCodificacaoBipolar(quadro);
            break;
    }

    CamadaDeAplicacaoReceptora(fluxoBrutoDeBits);
}

/*
    Decodifica��o binaria: ( Simplesmente o processo contrario ao de codifica��o )
        Transforma as amplitudes recebidas como 5v em 1
        e as amplitudes recebidas 0v em 0
 */
vector<int> CamadaFisicaReceptoraCodificacaoBinaria (vector<int> quadro) {
    int size_vector = (int) quadro.size();
    for (int inner = 0; inner < size_vector; inner++) {
        if (quadro[inner] == 0){
            quadro.at(inner) = 0;
        }else{
            quadro.at(inner) = 1;
        }
    }
    return quadro;
}

/*
    Decodifica��o manchester: ( Simplesmente o processo contrario ao de codifica��o )
        Aplica xor com 1 na amplitude recebida
        E aplica xor com 0 no resultado
 */
vector<int> CamadaFisicaReceptoraCodificacaoManchester (vector<int> quadro) {
    int size_vector = (int) quadro.size();
    for (int inner = 0; inner < size_vector; inner++) {
        quadro.at(inner) = quadro[inner] ^ 1;
        quadro.at(inner) = quadro[inner] ^ 0;
    }
    return quadro;
}

/*
@brief Decodifica��o bipolar.
Verifica se a amplitude recebida � diferente de 0, e caso seja, a atribui o valor 1.
@param quadro vetor de inteiros representando o quadro de bits recebido
@return vetor de inteiros representando o fluxo bruto de bits decodificado
*/
vector<int> CamadaFisicaReceptoraCodificacaoBipolar (vector<int> quadro) {
    int size_vector = (int) quadro.size();
    for (int inner = 0; inner < size_vector; inner++) {
        if (quadro[inner] != 0){
            quadro.at(inner) = 1;
        }
    }
    return quadro;
}

/*
    CamadaDeAplicacao Receptora:
        Aqui � feita a tradu��o dos bits em caracteres
 */
void CamadaDeAplicacaoReceptora (vector<int> quadro) {
    string mensagem, mensagemAux;
    int mensagemSize, size_vector = (int) quadro.size();
    int aux = 0, counter = 0;
    for (int inner = 0; inner < size_vector; inner++) {
        if (counter < 7){
            aux += quadro[inner] * pow(2,counter);
            counter++;
        }else if(counter >= 7){
            aux += quadro[inner] * pow(2,counter);
            mensagemAux += char(aux);
            aux = 0;
            counter = 0;
        }
    }
    mensagemSize = mensagemAux.length() - 1;
    for (int i = mensagemSize; i > -1; i--){
        mensagem += mensagemAux[i];
    }
    AplicacaoReceptora(mensagem);
}

void AplicacaoReceptora (string mensagem) {
    cout << "\nA mensagem recebida foi: " << mensagem << endl;
}
