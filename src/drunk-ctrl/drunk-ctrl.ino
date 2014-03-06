enum controlState {
    HALT, FORWARD, BACKWARD, FORWARDLEFT, FORWARDRIGHT, BACKWARDLEFT, BACKWARDRIGH;
}
enum drunkState { SOBER, TIPSY, DRUNK, HAMMERED };
enum influenceState { NORMAL, REVERSED, FLAPPING, WAVY, DELAYED };

sendControl(controlState, drunkState) 
{
   digitalWrite(FPIN,  HIGH);
}

void loop()
{    
    switch (getDrunkState())
    {
        case SOBER:
        case TIPSY:
        case DRUNK:
        case HAMMERED:
    }
}
