/*
// Autor - LizzardMedeiros
// Version - 1.0.0
//
// Atributos Iniciais
// Força - 6
// Agi - 0
// Int - 20
*/

//Variáveis importantes para o algorítmo de caminhada
int step = 0;
float placesX[2] = {0.0, 0.0};
float placesY[2] = {0.0, 0.0};

float enemy[2] = {0,0}; //x, y
float last_enconter = 0.0;

//Random Range
int randomRange(int min, int max){
    return ((random()%(max - min)) + min);
}

//Função de probabilidade
int dice(int chance){
    if(chance > 99) chance = 99;
    else if(chance < 0) chance = 0;
    return(random()%100 <= chance);
}
//Matematica
float max(float n1, float n2){
    return (n1 >= n2) ? n1 : n2;
}

int getMaxHp(){
    return 100 + (getSTR() * 10);
}

int getMaxAp(){
    return 100 + (getINT() * 10);
}

//Função de defesa
defense(){
    last_enconter = getSimTime();
    //Verifica se foi um golpe crítico
    if(getHp() < getMaxHp()*.8)
    {
        float target_head = getTargetHead();
        float xx = getTargetX();
        float yy = getTargetY();
        if(target_head > 0 && target_head < 180) teleport(xx - 2, yy);
        else if(target_head > 90 && target_head < 270) teleport(xx, yy - 2);
        else if(target_head > 180 && target_head < 360) teleport(xx + 2, yy);
        else teleport(xx, yy + 2);
    }
    while(!getCloseEnemy() || getSimTime() < last_enconter+5){
        turnToLastHit();
        attack();
        if(getHp() < getMaxHp()*.4) teleport(12.5, 12.5);
    }
}

//Função de ataque
attack(){
    if(getLowHp()){
        enemy[0] = getTargetX();
        enemy[1] = getTargetY();
        last_enconter = getSimTime();
    }
    fireball(enemy[0], enemy[1]);
    
    float t_dist = getDist(enemy[0], enemy[1]); //calcula a distância
    
    if(doYouSeeMe() && dice(30))
    {
        if(getAmbushTimeLeft() == 0) ambush();
        while(getAmbushTimeLeft() > .6) stepBack();
        attackRanged(enemy[0], enemy[1]);
        return;
    }else if(t_dist > 2 && getSafeRadius() > 8)
    {
        fireball(enemy[0], enemy[1]);
        if(dice(30)){
            speak("Queima!");
            stepRight();
            turnToTarget();
        }
        else if(dice(30)) speak("Só uma tostadinha!");
    }
}

//Função de movimento
//Algorítimo - Random Walk Monte Carlo adaptado
move(){
    int sr = getSafeRadius();
    
    if(getSafeRadius() < 10){// Vai para o centro caso a núvem tóxica esteja grande
        if(getX() != 12.5 && getY() != 12.5) moveTo(12.5, 12.5);
        else if(dice(85)) turnToAngle(random()%359);
    }
    
    if(getLowHp()){
        enemy[0] = getTargetX();
        enemy[1] = getTargetY();
        attack();
    }
    
    if(last_enconter > 0 && getSimTime() < last_enconter + 8 && isSafeHere()){
        dice(50) ? turnLeft(5) : turnRight(5);
    }    
    
    //Senão confere se o HP está crítico para se proteger
    else if(getHp() < getMaxHp()*.1)
    {
        if(!isSafeHere()) teleport(12.5, 12.5);
        turnLeft (45);
    }
    else{//Senão anda aleatóriamente
        if(placesX[0] == 0 && placesY[0] == 0){
            placesX[0] = (getX() < 12.5) ? getX()-2 : getX()+2;
            placesY[0] = (getY() < 12.5) ? getY()-2 : getY()+2;
        }
        
        else if(getSimTime() < 30 && !isTargetVisible())
        {
            float pp = randomRange(11.5, 13.5);
            getCloseEnemy() ? attack() : attackRanged(pp, pp);
            if(getHit()) defense();
        }
        
        //Random Walk Monte Carlo
        else if(getX() == placesX[step] && getY() == placesY[step]){
            float dx = (randomRange(-sr/2, sr/2));
            float dy = (randomRange(-sr/2, sr/2));
            float lx = placesX[step];
            float ly = placesY[step];
            step++;
            if(step == 2) step = 0;
            placesX[step] = (lx + dx);
            placesY[step] = (ly + dy);
            if(placesX[step] > 25-(sr/2) ) placesX[step] = 25-(sr/2);
            if(placesX[step] < (sr/2)) placesX[step] = (sr/2);
            if(placesY[step] > 25-(sr/2)) placesY[step] = 25-(sr/2);
            if(placesY[step] < (sr/2)) placesY[step] =(sr/2);
            //Rotação
            unsigned int i = getHead();
            if(dice(20)) while(!getHit()){
                turnToAngle(i);
                if(getCloseEnemy() || dice(5)) break;
                if(dice(50)) i += 30;
                else i -= 30;
            }
        }
        moveTo(placesX[step], placesY[step]);
    }
}

//Função loop
loop(){
    //Verifica se está sob ataque para se defender
    if(getHit()) defense();
    //Senão procura o inimigo com menor HP para atacá-lo
    else
    {
        if(getCloseEnemy() && isSafeHere()) attack();
        else move();
    }

    /*Prioriza Agi até o lvl 15. Depois Srt e por último Int*/
    if(getINT() < 50) upgradeINT(10);
    else if(getAGI() < 25) upgradeAGI(1);
    else if(dice(50)) upgradeSTR(1);
    else upgradeAGI(1);
}