#include "CamadaFisica.hpp"

volatile int tipoDeCondificacao = 2;
volatile int tipoDeDecodificacao = 2;

void AplicacaoTransmissora(void){
    string mensagem;
    cout << "Entre com a mensagem" << endl;
    cin >> mensagem;
    CamadaDeAplicacaoTransmissora(mensagem);
}

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
    Codificação binaria:
        Transforma os bit positivos (1) em 5v
        e os bit zeros (0) em 0v
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
    Codificação manchester:
        Aplica xor com 0 na amplitude
        E aplica xor com 1 no resultado
 */
vector<int> CamadaFisicaTransmissoraCodificacaoManchester (vector<int> quadro) {
    int size_vector = (int) quadro.size();
    for (int inner = 0; inner < size_vector; inner++) {
        quadro.at(inner) = quadro[inner] ^ 0;
        quadro.at(inner) = quadro[inner] ^ 1;
    }
    return quadro;
}

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

void MeioDeComunicacao (vector<int> fluxoBrutoDeBits) {
    vector<int> fluxoBrutoDeBitsPontoA, fluxoBrutoDeBitsPontoB;

    fluxoBrutoDeBitsPontoA = fluxoBrutoDeBits;
    int size = fluxoBrutoDeBitsPontoA.size();

    for (int i = 0; i < size; i++ ) {
        fluxoBrutoDeBitsPontoB.insert(fluxoBrutoDeBitsPontoB.begin() + i, fluxoBrutoDeBitsPontoA[i]);
    }

    CamadaFisicaReceptora(fluxoBrutoDeBitsPontoB);
}

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
