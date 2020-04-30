loop(){
    if (isSafeHere()) {
        if (getSafeRadius() > 12) {
            if (getINT() > 40) {
                upgradeSTR(2);
                upgradeINT(3);
            }
            else if (getINT() > 30) {
                upgradeSTR(1);
                upgradeINT(4);
            }
            else {
                upgradeINT(5);
            }
            if (getHit()) {
                speak("Adeus!");
                float x = getX();
                float y = getY();
                if ((x < 12) && (y < 12)) {
                    teleport(25, 25);
                }
                else if ((x < 12) && (y >= 12)) {
                    teleport(25, 1);
                }
                else if ((x >= 12) && (y < 12)) {
                    teleport(1, 25);
                }
                else if ((x >= 12) && (y >= 12)) {
                    teleport(1, 1);
                }
            }
            if (getCloseEnemy()) {
                if (getTargetHealth() > 0.3) {
                    speak("Queima!!!");    
                }
                else {
                    speak("You are dead.");
                }
                fireball(getTargetX(), getTargetY());   
            }
            else{
                moveTo(12, 14);
            }
        }
        else {
            upgradeSTR(5);
            if (getCloseEnemy()) {
                fireball(getTargetX(), getTargetY());   
            }
            else {
                moveTo(12, 14);
            }
        }
    }
        
    else {
            
        teleport(12, 14);
            
    }
}
