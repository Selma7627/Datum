#include <iostream>
#include <iomanip>
#include <ostream>
#include <vector>

class Datum{
    int dan, mjesec, godina;
    bool ValidanDatum(int d, int m, int g);
    public:
    enum Mjeseci {Januar = 1, Februar, Mart, April, Maj, Juni, Juli, August, Septembar,Oktobar, Novembar, Decembar};
    enum Dani {Ponedjeljak = 1, Utorak, Srijeda, Cetvrtak, Petak, Subota, Nedjelja};
    Datum(int d, int m, int g) { Postavi(d,m,g); }
    Datum(int d, Mjeseci m, int g): dan(d), mjesec(int(m)) { Postavi(d,m,g); }
    void Postavi(int d, int m, int g);
    void Postavi(int d, Mjeseci m, int g);
    int DajDan() const { return dan; }
    Mjeseci DajMjesec() const { return Mjeseci(mjesec); }
    int DajGodinu() const { return godina; }
    const char* DajImeMjeseca() const ;
    Dani DajDanUSedmici() const;
    const char* DajImeDanaUSedmici() const;
    Datum &operator ++();
    Datum &operator --();
    Datum operator ++(int);
    Datum operator --(int);
    Datum &operator +=(int);
    Datum &operator -=(int);
    friend Datum operator +(const Datum &d, int);
    friend Datum operator -(const Datum &d, int);
    friend std::ostream &operator <<(std::ostream &tok, const Datum &d){
        return tok << d.dan << ". " << d.DajImeMjeseca() << " " << d.godina << ". ("<< d.DajImeDanaUSedmici()<< ") ";
    }
};
bool Datum::ValidanDatum(int d, int m, int g){
    int broj_dana[]{31,28,31,30,31,30,31,31,30,31,30,31};
    if(g%4 == 0 && g%100!=0 || g % 400 == 0) broj_dana[1]++;
    if(d<1 || d>broj_dana[m-1] || m<1 || m>12 || g<1800) return false;
    return true;
}
void Datum::Postavi(int d, int m, int g){ 
    if(!ValidanDatum(d,m,g)) throw std::domain_error("Nelegalan datum");
    dan=d; mjesec=m; godina=g;
}

void Datum::Postavi(int d, Mjeseci m, int g){
    if(!ValidanDatum(d,int(m),g)) throw std::domain_error("Nelegalan datum");
    dan=d; mjesec=int(m); godina=g;
}

const char* Datum::DajImeMjeseca() const {
    const char *imena_mjeseci[]{"Januar","Februar","Mart","April","Maj","Juni","Juli","August","Septembar","Oktobar","Novembar","Decembar"};
    return imena_mjeseci[mjesec-1];
}

const char* Datum::DajImeDanaUSedmici() const {
    const char *ime_dana[]{"Ponedjeljak","Utorak","Srijeda","Cetvrtak","Petak","Subota","Nedjelja"};
    int suma=0;
    for(int i=1800; i<godina; i++){
        suma+=365;
        if((i%4 == 0 && i%100!=0) || i % 400 == 0) suma++;
    }
    int broj_dana[]{31,28,31,30,31,30,31,31,30,31,30,31};
    for(int i=1; i<mjesec; i++) suma+=broj_dana[i-1];
    if(((godina%4 == 0 && godina%100!=0 )|| godina % 400 == 0) && mjesec>2) suma++;
    
    suma+=dan+2;
   
    int redni_broj_dana=suma%7;

    if(redni_broj_dana==0) redni_broj_dana=7;
    return ime_dana[redni_broj_dana-1];
}

Datum::Dani Datum::DajDanUSedmici() const {
    int suma=0;
    for(int i=1800; i<godina; i++){
        suma+=365;
        if((i%4 == 0 && i%100!=0) || i % 400 == 0) suma++;
    }
    int broj_dana[]{31,28,31,30,31,30,31,31,30,31,30,31};
    for(int i=1; i<mjesec; i++) suma+=broj_dana[i-1];
    if(((godina%4 == 0 && godina%100!=0 )|| godina % 400 == 0) && mjesec>2) suma++;
    
    suma+=dan+2;
   
    int redni_broj_dana=suma%7;
    return Datum::Dani(redni_broj_dana);
}
Datum &Datum::operator ++(){
    int broj_dana[]{31,28,31,30,31,30,31,31,30,31,30,31};
    if(godina%4 == 0 && godina%100!=0 || godina % 400 == 0) broj_dana[1]++;
    if(dan==broj_dana[mjesec-1]) {
        dan=1;
        if(mjesec==12){
            mjesec=1;
            godina++;
        }
        else mjesec++;
    }
    else dan++;
    return *this;
}
Datum &Datum::operator --(){
    int broj_dana[]{31,28,31,30,31,30,31,31,30,31,30,31},d=dan,m=mjesec,g=godina;
    
    if(d==1) {
        if(m==1){
            m=12;
            g--;
        }
        else m--;
        d=broj_dana[m-1];
        if(m==2 && (g%4 == 0 && g%100!=0 || g % 400 == 0)) d++;
    }
    else d--;
    Postavi(d,m,g);
    return *this;
}
Datum Datum::operator ++(int){
    Datum d=*this;
    this->operator ++();
    return d;
}
Datum Datum::operator --(int){
    Datum d=*this;
    this->operator --();
    return d;
}

Datum operator +(const Datum &d, int n){
    Datum d1=d;
    if(n<0) {
    n*=-1;
    d1=operator -(d1,n);
    }
    else {
    int broj_dana[]{31,28,31,30,31,30,31,31,30,31,30,31};
    int broj_dana_u_godini=365;
    while(n>=broj_dana_u_godini){
        broj_dana_u_godini=365;
        if((d1.mjesec==1 || d1.mjesec==2 && d1.dan!=29) && (d1.godina%4 == 0 && d1.godina%100!=0 || d1.godina % 400 == 0)) 
            broj_dana_u_godini++;
        n-=broj_dana_u_godini;
        d1.godina++;
    }

    while(n>broj_dana[d1.mjesec-1]){
        n-=broj_dana[d1.mjesec-1];
        if(d1.mjesec==12) { d1.mjesec=0; d1.godina++; }
        d1.mjesec++;
    }

    for(int i=0; i<n; i++){
        if(d1.dan<broj_dana[d1.mjesec]) d1.dan++;
        else { d1.dan=1; 
            if(d1.mjesec==12){
                d1.mjesec=1; d1.godina++;
            }
            else d1.mjesec++;
         }
    }
    }
    return d1;

}

Datum operator -(const Datum &d, int n){
    Datum d1=d;
    if(n<0) {
        n*=-1;
        d1=operator +(d1,n);
    }
    else {
    int broj_dana[]{31,28,31,30,31,30,31,31,30,31,30,31};
    int broj_dana_u_godini=365;
    while(n>=broj_dana_u_godini){
        broj_dana_u_godini=365;
        if((d1.mjesec==1 || d1.mjesec==2 && d1.dan!=29) && (d1.godina%4 == 0 && d1.godina%100!=0 || d1.godina % 400 == 0)) 
            broj_dana_u_godini++;
        n-=broj_dana_u_godini;
        d1.godina--;
    }
    if (d1.godina%4 == 0 && d1.godina%100!=0 || d1.godina % 400 == 0) broj_dana[1]++;
    if(d1.mjesec==1 && n>broj_dana[11]) { d1.mjesec=12; d1.godina--; n-=broj_dana[d1.mjesec-1];}
    while(n>broj_dana[d1.mjesec-2]){
        if(d1.mjesec==1) { d1.mjesec=12; d1.godina--; n-=broj_dana[d1.mjesec-1];}
        else d1.mjesec--;
        n-=broj_dana[d1.mjesec-2];
    }
    
    for(int i=0; i<n; i++){
        if(d1.dan>1) d1.dan--;
        else {
            if(d1.mjesec==1){
                d1.mjesec=12; d1.godina--;
                d1.dan=broj_dana[d1.mjesec-1];
            }
            else {
                d1.mjesec--; 
                d1.dan=broj_dana[d1.mjesec-1];
            }
         }
    }
    }
    
    return d1;
}

Datum &Datum::operator +=(int n){
    if(n<0) {
    n*=-1;
    *this-=n;
    }
    else {
    int broj_dana[]{31,28,31,30,31,30,31,31,30,31,30,31};
    int broj_dana_u_godini=365;
    while(n>=broj_dana_u_godini){
        broj_dana_u_godini=365;
        if((this->mjesec==1 || this->mjesec==2 && this->dan!=29) && (this->godina%4 == 0 && this->godina%100!=0 || this->godina % 400 == 0)) 
            broj_dana_u_godini++;
        n-=broj_dana_u_godini;
        this->godina++;
    }

    while(n>broj_dana[mjesec-1]){
        n-=broj_dana[mjesec-1];
        if(mjesec==12) { mjesec=0; godina++; }
        mjesec++;
    }

    for(int i=0; i<n; i++){
        if(dan<broj_dana[mjesec]) dan++;
        else { dan=1; 
            if(mjesec==12){
                mjesec=1; godina++;
            }
            else mjesec++;
         }
    }
    }
    return *this;
}
Datum &Datum::operator -=(int n){
    if(n<0) {
        n*=-1;
        *this+=n;
    }
    else {
    int broj_dana[]{31,28,31,30,31,30,31,31,30,31,30,31};
    int broj_dana_u_godini=365;
    while(n>=broj_dana_u_godini){
        broj_dana_u_godini=365;
        if((mjesec==1 || mjesec==2 && dan!=29) && (godina%4 == 0 && godina%100!=0 || godina % 400 == 0)) 
            broj_dana_u_godini++;
        n-=broj_dana_u_godini;
        godina--;
    }
    if (godina%4 == 0 && godina%100!=0 || godina % 400 == 0) broj_dana[1]++;
    if(mjesec==1 && n>broj_dana[11]) { mjesec=12; godina--; n-=broj_dana[mjesec-1];}
    while(n>broj_dana[mjesec-2]){
        if(mjesec==1) { mjesec=12; godina--; n-=broj_dana[mjesec-1];}
        else mjesec--;
        n-=broj_dana[mjesec-2];
    }
    
    for(int i=0; i<n; i++){
        if(dan>1) dan--;
        else {
            if(mjesec==1){
                mjesec=12; godina--;
                dan=broj_dana[mjesec-1];
            }
            else {
                mjesec--; 
                dan=broj_dana[mjesec-1];
            }
         }
    }
    }
    
    return *this;
}
int main ()
{
     

    	return 0;
}
