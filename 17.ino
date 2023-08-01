const int stepPin = 11;
const int dirPin = 12;
const int enaPin = 4; // Enable pin for the stepper motor driver
const int stepsPerRotation = 200;
const int stationSecond = 5;
// 1800 derece
const int stopDelay = 1000; // 1 second //delme hızlı çalışması için 1000 yap
const int totalItems = 20;
const int stepsPerItem = stepsPerRotation / totalItems;
// Current state of the finite state machine
enum State
{
    MOVING_TO_STOP,
    PROCESSING_ITEM,
    IDLE
};

State currentState = MOVING_TO_STOP;
int currentItem = 1;
int currentStep = 0;    // Variable to keep track of the current step count
int currentStation = 1; // Variable to keep track of the current station (motor) number

void setup()
{
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enaPin, OUTPUT);
    digitalWrite(dirPin, HIGH); // Set direction to clockwise (you can change this based on your setup)
    digitalWrite(enaPin, LOW);  // Enable the motor driver by setting ENA pin HIGH
    Serial.begin(9600);
}

void loop()
{
    switch (currentState)
    {
    case MOVING_TO_STOP:
        moveStepperToNextStop();
        currentState = PROCESSING_ITEM;
        break;

    case PROCESSING_ITEM:
        // Perform processing for the current item here
        Serial.print("Processing Item: ");
        Serial.println(currentItem);
        Serial.print("CurrentItem: ");
        Serial.println(currentItem);
        // Wait for the processing time (you can replace this with the actual processing code)
        delay(stopDelay);

        // Move to the next item or stop if all items are processed
        currentItem++;
        if (currentItem > totalItems)
        {
            Serial.print("currentItem > totalItems");
            currentState = IDLE;
            Serial.print(currentState);
        }
        else
        {
            Serial.print("currentItem <= totalItems");
            currentState = MOVING_TO_STOP;
            Serial.print(currentState);
        }

        // Check if the current item is the 5th or its multiples
        if (currentItem % 5 == 0)
        {
            // Start the new motor for the current station
            startMotorForCurrentStation();
        }

        break;

    case IDLE:
        // The processing is done for all items, you can add additional actions here or keep it idle
        // For example, you can wait for a button press to start the sequence again.
        Serial.print("PROCESS COMPLETED");
        break;
    }
}

void moveStepperToNextStop()
{
    int targetStep = (currentItem * stepsPerItem) % stepsPerRotation;
    Serial.println("targetStep: ");
    Serial.println(targetStep);
    int stepSize = (targetStep >= currentStep) ? 1 : -1;

    while (currentStep != targetStep)
    {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000); // set rotate speed (Adjust this value as needed)
        digitalWrite(stepPin, LOW);
        delayMicroseconds(1000); // set rotate speed (Adjust this value as needed)
        currentStep += stepSize;
        if (currentStep < 0)
        {
            currentStep = stepsPerRotation - 1;
        }
        else if (currentStep >= stepsPerRotation)
        {
            currentStep = 0;
        }
    }

    // Print the current step count after each movement
    Serial.print("Current Step: ");
    Serial.println(currentStep);
}

void startMotorForCurrentStation()
{
    // You can implement the code to start the motor corresponding to the currentStation here.
    // For example, you can use digitalWrite to control the motor's enable pin.
    // For demonstration purposes, we'll just print a message indicating which station (motor) has started.
    Serial.print("Starting motor for Station ");
    Serial.println(currentStation);

    // Increase the station number for the next cycle
    currentStation++;
    // If we exceed the total number of stations, reset back to 1
    if (currentStation > totalItems)
    {
        currentStation = 1;
    }
}
