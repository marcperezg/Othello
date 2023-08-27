#include "JugadorIA.hpp"

JugadorIA::JugadorIA()
{
    num_turno = 0;
    setBasics();
}

JugadorIA::JugadorIA(int &n)
{
    num_turno = n;
    setBasics();
}

void JugadorIA::setBasics()
{
    nombre = "PACO";
    puntuacion = 0;
    partidas_ganadas = 0;
    jugadasComprobadas = 0;
    profundidad = 6;
    inicializarHash();
}

void JugadorIA::TotalScore(const Tablero &t)
{
    puntuacion += t.GetPuntuacion(num_turno);
}

void JugadorIA::SetNumTurno(int &n)
{
    num_turno = n;
}

void JugadorIA::MostrarNombreJugador()
{
    std::cout << nombre;
}

void JugadorIA::PartidaGanada()
{
    partidas_ganadas++;
}

int JugadorIA::NumeroPartidasGanadas()
{
    return partidas_ganadas;
}

int JugadorIA::GetPuntuacion()
{
    return puntuacion;
}

std::vector<JugadorIA::Move> JugadorIA::getMoves(const Tablero t)
{
    std::vector<Move> movPosibles;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (t.PosicionValida(i, j))
            {
                movPosibles.push_back({i, j});
            }
        }
    }
    return movPosibles;
}

void JugadorIA::miTurno(Tablero &t)
{
    jugadasComprobadas = 0;
    Move mejorMov = move(t);
    t.SetFicha(mejorMov.fil, mejorMov.col);
    std::cout << "Jugadas Comprobadas = " << jugadasComprobadas << std::endl;
}

JugadorIA::Move JugadorIA::move(Tablero t)
{
    int valor;
    std::vector<Move> moves = getMoves(t);
    Move mejorMov = moves[0];
    std::vector<std::pair<Move, int>> candidats;

    #pragma omp parallel for
    for (int i = 0; i < moves.size(); i++)
    {
        Tablero nuevoT = t;
        nuevoT.SetFicha(moves[i].fil, moves[i].col);
        if (!nuevoT.FinPartida())
        {
            if (nuevoT.Ganador() == 2)
            {
                mejorMov = moves[i];
                valor = 99999;
            }
        }
        candidats.push_back(std::pair<Move, int> (moves[i], Min_Valor(nuevoT, profundidad - 1, -99999, 99999)));
    }
    mejorMov = candidats[0].first;
    valor = candidats[0].second;
    for(int i = 1; i<candidats.size(); i++){
        if (valor < candidats[i].second)
        {
            mejorMov = candidats[i].first;
            valor = candidats[i].second;
        }
    }
    return mejorMov;
}

int JugadorIA::max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

int JugadorIA::min(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}

int JugadorIA::minmax(Tablero t, std::vector<Move> moves, int i, int profunditat, int alfa, int beta, int opcion)
{
    int valor = 99999 * opcion;
    if (i == moves.size())
        return valor;
    valor = minmax(t, moves, i + 1, profunditat, alfa, beta, opcion);
    Tablero nuevoT = t;
    nuevoT.SetFicha(moves[i].fil, moves[i].col);
    if (!nuevoT.FinPartida())
    {
        if (nuevoT.Ganador() == 2)
            return 99999;
        else
            return -99999;
    }
    if (opcion == -1)
    {
        valor = max(valor, Min_Valor(nuevoT, profunditat - 1, alfa, beta));
        if (beta <= valor)
            return valor;
        alfa = max(valor, alfa);
    }
    else if (opcion == 1)
    {
        valor = min(valor, Max_Valor(nuevoT, profunditat - 1, alfa, beta));
        if (valor <= alfa)
            return valor;
        beta = min(valor, beta);
    }
    return valor;
}

int JugadorIA::Min_Valor(Tablero t, int profunditat, int alfa, int beta)
{
    std::vector<Move> moves = getMoves(t);
    if (profunditat == 0 || moves.empty())
    {
        return ValorRetorn(t);
    }
    return minmax(t, moves, 0, profunditat, alfa, beta, 1);
}

int JugadorIA::Max_Valor(Tablero t, int profunditat, int alfa, int beta)
{
    std::vector<Move> moves = getMoves(t);
    if (profunditat == 0 || moves.empty())
    {
        return ValorRetorn(t);
    }
    return minmax(t, moves, 0, profunditat, alfa, beta, -1);
}

int JugadorIA::ValorRetorn(Tablero t)
{
    std::uint64_t index = calcular_hash(t, 0, 0) % 99999;
    jugadasComprobadas++;
    if (AlmacenHash[index] == 0){
        #pragma omp critical 
        { 
            AlmacenHash[index] = heuristica(t); 
        }
    }
    return AlmacenHash[index];
}

JugadorIA::Move JugadorIA::contarFichas(Tablero t, int fil)
{
    Move contFT = {0, 0};
    if (fil == 8)
        return contFT;
    else
    {
        contFT = contarFichas(t, fil + 1);
        int col = 0;
        while (col < 8)
        {
            if (t.GetPosicion(fil, col) == 2)
                contFT.fil += valores[fil][col] + 1;
            else if (t.GetPosicion(fil, col) == 1)
                contFT.col += valores[fil][col] + 1;
            col++;
        }
    }
    return contFT;
}

int JugadorIA::heuristica(Tablero t)
{
    if (!t.FinPartida())
    {
        if (t.Ganador() == 2)
            return 99999;
        else
            return -99999;
    }
    Move contF = contarFichas(t, 0);
    int puntuacionJugador = contF.fil, puntuacionOponente = contF.col;
    return puntuacionJugador - puntuacionOponente;
}


std::uint64_t JugadorIA::calcular_hash(Tablero t, int i, int j)
{
    std::uint64_t hash = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            hash ^= matrices_de_hash[t.GetPosicion(i, j)][i][j];
        }
    }
    return hash;
}


void JugadorIA::inicializarHash()
{
    std::srand(std::time(nullptr));
    for (int k = 0; k < 3; k++)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                matrices_de_hash[k][i][j] = std::rand();
            }
        }
    }
}
