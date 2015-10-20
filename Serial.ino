 // Write out data to serial monitor
void serialOut() {
    Serial.print("Raw height: ");
    Serial.print(inches);
    Serial.print("   ");
  
    Serial.print("Offset Value: ");
    Serial.print(offsetValue);
    Serial.print("   ");
//Take inches value and subtract the offset to get height over ground.
    Serial.print("Calc height: ");
    Serial.print(inches - offsetValue);
    Serial.println();
}
