#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <math.h>

using namespace std;

void AplicacaoTransmissora();

void CamadaDeAplicacaoTransmissora (string mensagem);

void CamadaFisicaTransmissora (vector<int> quadro);

vector<int>  CamadaFisicaTransmissoraCodificacaoBinaria (vector<int> quadro);

vector<int> CamadaFisicaTransmissoraCodificacaoManchester (vector<int> quadro);

vector<int> CamadaFisicaTransmissoraCodificacaoBipolar (vector<int> quadro);

void MeioDeComunicacao (vector<int> fluxoBrutoDeBits );

void CamadaFisicaReceptora (vector<int> quadro);

vector<int> CamadaFisicaReceptoraCodificacaoBinaria (vector<int> quadro);

vector<int> CamadaFisicaReceptoraCodificacaoManchester (vector<int> quadro);

vector<int> CamadaFisicaReceptoraCodificacaoBipolar (vector<int> quadro);

void CamadaDeAplicacaoReceptora (vector<int> quadro);

void AplicacaoTransmissora (string mensagem);
