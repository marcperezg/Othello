#ifndef _JUGADOR_IA_
#define _JUGADOR_IA_

#include "TableroClass.h"
#include <vector>
#include <iostream>
#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <array>
#include <cstdint>
#include <string>

class JugadorIA
{
private:
    long jugadasComprobadas;
    std::string nombre;
    int num_turno;
    int puntuacion;
    int partidas_ganadas;
    int profundidad;
    std::uint64_t AlmacenHash[99999];
    std::array<std::array<std::array<std::uint64_t, 8>, 8>, 3> matrices_de_hash;
    int valores[8][8] = {
        {99, -8, 8, 6, 6, 8, -8, 99},
        {-8, -50, -4, -3, -3, -4, -50, -8},
        {8, -4, 7, 4, 4, 7, -4, 8},
        {6, -3, 4, 0, 0, 4, -3, 6},
        {6, -3, 4, 0, 0, 4, -3, 6},
        {8, -4, 7, 4, 4, 7, -4, 8},
        {-8, -50, -4, -3, -3, -4, -50, -8},
        {99, -8, 8, 6, 6, 8, -8, 99}};

    struct Move
    {
        int fil;
        int col;
    };

    int max(int a, int b);
    int min(int a, int b);

    void setBasics();
    Move move(Tablero t);
    std::vector<Move> getMoves(Tablero t);
    int minmax(Tablero t, std::vector<Move> moves, int i, int profunditat, int alfa, int beta, int opcion);
    int Min_Valor(Tablero t, int profunditat, int alfa, int beta);
    int Max_Valor(Tablero t, int profunditat, int alfa, int beta);
    Move contarFichas(Tablero t, int fil);
    int heuristica(Tablero t);
    int ValorRetorn(Tablero t);
    std::uint64_t calcular_hash(Tablero t, int i, int j);
    void inicializarHash();

public:
    JugadorIA();
    JugadorIA(int &n);
    void TotalScore(const Tablero &t);
    void SetNumTurno(int &n);
    void MostrarNombreJugador();
    void PartidaGanada();
    int NumeroPartidasGanadas();
    int GetPuntuacion();
    void miTurno(Tablero &t);
};

#endif