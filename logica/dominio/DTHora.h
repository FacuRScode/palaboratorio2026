#ifndef DTHORA_H
#define DTHORA_H

class DTHora{
    private:
        int hora, minuto, segundo;
    public:
    DTHora(int hora, int minuto, int segundo);

    int getHora();
    int getMinuto();
    int getSegundo();

    void setHora(int hora);
    void setMinuto(int minuto);
    void setSegundo(int segundo);
};
#endif