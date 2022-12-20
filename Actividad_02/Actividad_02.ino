/*
Alumno: Roberto Miguel Salas Ayala
Grupo: 3MM6

A continuacion se puede ver que contamos con dos tareas de distinta prioridad, la tarea de mayor prioridad se encuentra
al incio de la ejecución y la detenemos con una sección del código que se encuentra en el loop. Finalmente con la
tarea de menor prioridad realizamos la impresión de un caracter, veremos que este patron de impresion se hara muchas veces
en el monitor serie.*/
// Utilice solo core 1 para fines de demostración
#si CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#más
  Const estáticoBaseType_t app_cpu =1;
#endif
// Mensaje inicial
constcharmsg[] ="Mensaje inicial --- UPIIZ ---";

estático TaskHandle_t task_1 =NULL;
estático TaskHandle_t task_2 =NULL;
//*****************************************************************************
// Tareas
// Tarea con baja prioridad
void startTask1(void *parameter) {
  
  // Conteo de numero de caracteres
  int msg_len = strlen(msg);
  
  // Imprimir mensaje por terminal
  mientras(1) {
Serial. println();
    for (int i = 0; i < msg_len; i++) {
Serial. imprimir(msg[i]);
    }
Serial. println();
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}
// Tarea con alta prioridad
void startTask2(void *parameter) {
  mientras(1) {
Serial. Impresión('*');
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}
//*****************************************************************************
// Principal
 
Configuración vacía() {
  // Serial lento para poder visualizar
Serial. begin(300);

  // Retardo antes de iniciar
  vTaskDelay(1000/portTICK_PERIOD_MS);
Serial. println();
Serial. println("---FreeRTOS UPIIZ ---");

  // Imprimir prioridad propia
Serial. print("Configuracion y ciclo de tareas corriendo");
Serial. print(xPortGetCoreID());
Serial. print("con prioridad");
Serial. println(uxTaskPriorityGet(NULL));

 // Task corre por siempre
  xTaskCreatePinnedToCore(startTask1,
                          "Tarea 1",
                          1024,
                          NULL,
                          1,
                          &task_1,
                          app_cpu);

  // Tarea con alta prioridad corre solo por una vez
  xTaskCreatePinnedToCore(startTask2,
                          "Tarea 2",
                          1024,
                          NULL,
                          2,
                          &task_2,
                          app_cpu);
}

bucle void() {
  
  // Suspendemos la tarea de alta prioridad por algunos periodos de tiempo
  for (int i = 0; i < 3; i++) {
    vTaskSuspend(task_2);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }
  
  // Borramos tarea de baja prioridad
  if (task_1 != NULL) {
    vTaskDelete(task_1);
    task_1 = NULL;
  }
}