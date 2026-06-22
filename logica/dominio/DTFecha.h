#ifndef DTFECHA_H
#define DTFECHA_H

using namespace std;
class DTFecha{
    private:
        int dia, mes, anio;
    public:
    DTFecha(int dia, int mes, int anio);

    int getDia() const;
    int getMes() const;
    int getAnio() const;

    void setDia(int dia);
    void setMes(int mes);
    void setAnio(int anio);

    bool operator<=(const DTFecha& otra) const;
    bool operator>=(const DTFecha& otra) const;
    bool operator<(const DTFecha& otra) const;
};
#endif
