/*
Alumno: Roberto Miguel Salas Ayala
Grupo: 3MM6

El proposito del programa es ejecutar dos tareas, en donde si la tarea 1 no envia un valor, la tarea 2 no sera ejecutada debido
a que esta depende de la otra. En la tarea 1 realizamos una lectura con el puerto serial y en la tarea 2 se imprime lo de
la lectura, esto sera hasta que senvie un reset para todo el programa.*/
// Utilice solo core 1 para fines de demostración
#si CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#más
  Const estáticoBaseType_t app_cpu =1;
#endif

Mensaje de cadena ="";
static char incomingChar;
estático TaskHandle_t task_1 =NULL;
estático TaskHandle_t task_2 =NULL;

//*****************************************************************************
// Tareas

// Aqui se tiene lo ques la tarea A
void startTask1(void *parameter) {
  mientras(1) {
    if(Serial. disponible()){
      charincomingChar = Serial. leer();
      if(incomingChar !='\n'){
mensaje +=String(incomingChar);
      }
      más{
mensaje ="";
      }
Serial. write(incomingChar);
  }
}
}

// Aqui se tiene la tarea B
void startTask2(void *parameter) {
  mientras(1) {
Serial. println(incomingChar);
         if(message =="Reset"){
incomingChar='\n';
         }
    }
  }
  
//*****************************************************************************
// Principal
 
Configuración vacía() {
  
  // Serial lento para poder visualizar
Serial. begin(300);

  // Retardo antes de iniciar
  vTaskDelay(1000/portTICK_PERIOD_MS);
  
 // Tarea 1
  xTaskCreatePinnedToCore(startTask1,
                          "Tarea 1",
                          1024,
                          NULL,
                          1,
                          &task_1,
                          app_cpu);
                          
  // Tarea 2
  xTaskCreatePinnedToCore(startTask2,
                          "Tarea 2",
                          1024,
                          NULL,
                          1,
                          &task_2,
                          app_cpu);
}

bucle void() {
  
}