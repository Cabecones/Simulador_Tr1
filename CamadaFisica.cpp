#include "CamadaFisica.hpp"

// Variáveis globais para definir o tipo de codificação e decodificação utilizadas
volatile int tipoDeCondificacao = 2;
volatile int tipoDeDecodificacao = 2;

/*
  Função que solicita uma mensagem ao usuário e a envia para a camada de aplicação transmissora
 */
void AplicacaoTransmissora(void){
    string mensagem;
    cout << "Entre com a mensagem" << endl;
    cin >> mensagem;
    CamadaDeAplicacaoTransmissora(mensagem);
}

/*
 * Função que transforma a mensagem em um vetor de bits
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
 * Função que seleciona o tipo de codificação a ser utilizada e envia o quadro de bits para o meio de comunicação
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
 * Função que codifica os bits com a codificação binária
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
Função que codifica o quadro com a técnica Manchester.
A codificação é feita aplicando XOR com 1 na amplitude recebida e depois com 0 no resultado.
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
Função que codifica o quadro com a técnica Bipolar.
A codificação é feita alternando entre 1 e -1 quando o bit é igual a 1.
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
Função que envia o quadro pelo meio de comunicação.
O quadro é replicado e enviado para a camada física receptora.
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
@brief Função responsável por decodificar o quadro de acordo com o tipo de codificação.
A função realiza a decodificação do quadro de acordo com o tipo de codificação escolhido.
A decodificação pode ser binária, Manchester ou bipolar.
Em seguida, a função chama a Camada de Aplicação Receptora para processar o fluxo bruto de bits.
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
    Decodificação binaria: ( Simplesmente o processo contrario ao de codificação )
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
    Decodificação manchester: ( Simplesmente o processo contrario ao de codificação )
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
@brief Decodificação bipolar.
Verifica se a amplitude recebida é diferente de 0, e caso seja, a atribui o valor 1.
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
        Aqui é feita a tradução dos bits em caracteres
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
