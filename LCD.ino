void sendtoLCD(){
  sprintf(altstring,"%4d",actual); // create strings from the numbers
  sprintf(offsetstring,"%4d",offsetValue); // right-justify to 4 spaces
  LCD.write(254); // cursor to 7th position on first line
  LCD.write(134);
  LCD.write(altstring); // write out the first line
  LCD.write(254); // cursor to 7th position on second line
  LCD.write(198);
  LCD.write(offsetstring); // write out the TEMP value
  }
