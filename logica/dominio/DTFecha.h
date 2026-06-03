#ifndef DETFECHA_H
#define DTFECHA_H

using namespace std;
class DTFecha{
    private:
        int dia, mes, anio;
    public:
    DTFecha(int dia, int mes, int anio);

    int getDia();
    int getMes();
    int getAnio();

    void setDia(int dia);
    void setMes(int mes);
    void setAnio(int anio);
};
#endif