PAV - P3: detección de pitch
============================

Esta práctica se distribuye a través del repositorio GitHub [Práctica 3](https://github.com/albino-pav/P3).
Siga las instrucciones de la [Práctica 2](https://github.com/albino-pav/P2) para realizar un `fork` de la
misma y distribuir copias locales (*clones*) del mismo a los distintos integrantes del grupo de prácticas.

Recuerde realizar el *pull request* al repositorio original una vez completada la práctica.

Ejercicios básicos
------------------

- Complete el código de los ficheros necesarios para realizar la detección de pitch usando el programa
  `get_pitch`.

   * Complete el cálculo de la autocorrelación e inserte a continuación el código correspondiente.
   
   ```cpp
   void PitchAnalyzer::autocorrelation(const vector<float> &x, vector<float> &r) const
   {

    for (unsigned int l = 0; l < r.size(); ++l)
    {
      /// \TODO Compute the autocorrelation r[l]
      /// \DONE
      float sum = 0;
      for (unsigned int i = 0; i < frameLen - l; i++)
      {
        sum += x[i] * x[i + l];
      }
      r[l] = sum / frameLen;
      sum = 0;
    }

    if (r[0] == 0.0F) //to avoid log() and divide zero
      r[0] = 1e-10;
   }
   ```

   * Inserte una gŕafica donde, en un *subplot*, se vea con claridad la señal temporal de un segmento de
     unos 30 ms de un fonema sonoro y su periodo de pitch; y, en otro *subplot*, se vea con claridad la
	 autocorrelación de la señal y la posición del primer máximo secundario.

	 NOTA: es más que probable que tenga que usar Python, Octave/MATLAB u otro programa semejante para
	 hacerlo. Se valorará la utilización de la librería matplotlib de Python.

   <img src="Autocorrelacion.png" width="640" align="center">

   * Determine el mejor candidato para el periodo de pitch localizando el primer máximo secundario de la
     autocorrelación. Inserte a continuación el código correspondiente.
      vector<float>::const_iterator iR = r.begin(), iRMax = iR;
    
    ```cpp
    float max_value = 0;
    /// \TODO
    /// Find the lag of the maximum value of the autocorrelation away from the origin.<br>
    /// Choices to set the minimum value of the lag are:
    ///    - The first negative value of the autocorrelation. 
    ///    - The lag corresponding to the maximum value of the pitch. (npitch_min condition)
    ///	   .
    /// In either case, the lag should not exceed that of the minimum value of the pitch.
    /// \DONE
    
    bool negative_found = false;
    //setting the minimum value of the lag
    while(!negative_found && iR - r.begin() < npitch_min) {
      if (*iR < 0) {
        negative_found = true;
      }
      iR++;
    }
    //finding the maximum of the autocorrelation for npitch_min <= n <= npitch_max
    while(iR - r.begin() <= npitch_max) {
      if (*iR >= max_value) {
        iRMax = iR;
        max_value = *iR;
      }
      iR++;
    }

    unsigned int lag = iRMax - r.begin();
    ```

   * Implemente la regla de decisión sonoro o sordo e inserte el código correspondiente.

   ```cpp
    bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm) const
  {
    /// \TODO Implement a rule to decide whether the sound is voiced or not.
    /// * You can use the standard features (pot, r1norm, rmaxnorm),
    ///   or compute and use other ones.
    /// \DONE

    if ((r1norm > weight1 || rmaxnorm > weight2) && pot > weight3 && r1norm > weight4 && rmaxnorm > weight5)
    {
      return false;
    }
    return true;
  }
   ```
En el siguiente diagrama se muestra la regla de decisión escogida. Además de la mostrada en el dibujo, para decidir sonoridad se tiene que cumplir que la potencia sea mayor al umbral weight3. 

<img src="unvoiced_dibujo.jfif" width="350" align="center">

- Una vez completados los puntos anteriores, dispondrá de una primera versión del detector de pitch. El 
  resto del trabajo consiste, básicamente, en obtener las mejores prestaciones posibles con él.

  * Utilice el programa `wavesurfer` para analizar las condiciones apropiadas para determinar si un
    segmento es sonoro o sordo. 
	
	  - Inserte una gráfica con la detección de pitch incorporada a `wavesurfer` y, junto a ella, los 
	    principales candidatos para determinar la sonoridad de la voz: el nivel de potencia de la señal
		(r[0]), la autocorrelación normalizada de uno (r1norm = r[1] / r[0]) y el valor de la
		autocorrelación en su máximo secundario (rmaxnorm = r[lag] / r[0]).

		Puede considerar, también, la conveniencia de usar la tasa de cruces por cero.

	    Recuerde configurar los paneles de datos para que el desplazamiento de ventana sea el adecuado, que
		en esta práctica es de 15 ms.

    <img src="Comparacion_pitch_parametros.PNG" width="750" align="center">
    
      De arriba a abajo tenemos: Pitch(Hz), Potencia (dB), r[1]/r[0], r[lag]/r[0].


      - Use el detector de pitch implementado en el programa `wavesurfer` en una señal de prueba y compare
	    su resultado con el obtenido por la mejor versión de su propio sistema.  Inserte una gráfica
		ilustrativa del resultado de ambos detectores.
    
    <img src="PitchWavesurfer_vs_Pitch.PNG" width="750" align="center">

    De arriba a abajo tenemos: Pitch_Wavesurfer(Hz), Pitch_Sistema(Hz), Señal WAV.

  
  * Optimice los parámetros de su sistema de detección de pitch e inserte una tabla con las tasas de error
    y el *score* TOTAL proporcionados por `pitch_evaluate` en la evaluación de la base de datos 
	`pitch_db/train`..
	
| TOTAL| 91.17 %|
| -- | -- |
| Unvoiced frames as voiced |      275/7045 (3.90 %) |
|Voiced frames as unvoiced  |    364/4155 (8.76 %) |
|Gross voiced errors (+20.00 %) |109/3791 (2.88 %) |
|MSE of fine errors  |   2.28 % |

Para optimizar los parámetros de nuestro programa, hemos creado un script llamado `train_get_pitch.sh`, que sirve para cambiar fácilmente los valores de los umbrales.
   

Ejercicios de ampliación
------------------------

- Usando la librería `docopt_cpp`, modifique el fichero `get_pitch.cpp` para incorporar los parámetros del
  detector a los argumentos de la línea de comandos.
  
  Esta técnica le resultará especialmente útil para optimizar los parámetros del detector. Recuerde que
  una parte importante de la evaluación recaerá en el resultado obtenido en la detección de pitch en la
  base de datos.

  * Inserte un *pantallazo* en el que se vea el mensaje de ayuda del programa y un ejemplo de utilización
    con los argumentos añadidos.
    
     <img src="get_pitch_help.PNG" width="500" align="center">

- Implemente las técnicas que considere oportunas para optimizar las prestaciones del sistema de detección
  de pitch.

  Entre las posibles mejoras, puede escoger una o más de las siguientes:

  * Técnicas de preprocesado: filtrado paso bajo, *center clipping*, etc.
  * Técnicas de postprocesado: filtro de mediana, *dynamic time warping*, etc.
  * Métodos alternativos a la autocorrelación: procesado cepstral, *average magnitude difference function*
    (AMDF), etc.
  * Optimización **demostrable** de los parámetros que gobiernan el detector, en concreto, de los que
    gobiernan la decisión sonoro/sordo.
  * Cualquier otra técnica que se le pueda ocurrir o encuentre en la literatura.

  Encontrará más información acerca de estas técnicas en las [Transparencias del Curso](https://atenea.upc.edu/pluginfile.php/2908770/mod_resource/content/3/2b_PS%20Techniques.pdf)
  y en [Spoken Language Processing](https://discovery.upc.edu/iii/encore/record/C__Rb1233593?lang=cat).
  También encontrará más información en los anexos del enunciado de esta práctica.
  
  Incluya, a continuación, una explicación de las técnicas incorporadas al detector. Se valorará la
  inclusión de gráficas, tablas, código o cualquier otra cosa que ayude a comprender el trabajo realizado.

  También se valorará la realización de un estudio de los parámetros involucrados. Por ejemplo, si se opta
  por implementar el filtro de mediana, se valorará el análisis de los resultados obtenidos en función de
  la longitud del filtro.

  Hemos utilizado tanto la técnica de preprocesado del *center clipping* como la de posprocesado del filtro de mediana.

  El código del center clipping es el siguiente:

  ```cpp
  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
  /// \DONE
  // Hacemos center-clipping
  vector<float>::iterator iX;
  #if 1
  float umbral = 0.005;
  for(iX = x.begin(); iX < x.end(); iX++) {
    if(*iX <= umbral && *iX >= -umbral) {
       *iX = 0;
    }
  }
  #endif
  ```
  El umbral lo hemos escogido de forma que las tramas sonoras no pierdan la periodicidad.

  El código de filtro de mediana es el siguente:

  ```cpp
	/// \TODO
	/// Postprocess the estimation in order to supress errors. For instance, a median filter
	/// or time-warping may be used.
	/// Utilizamos el median filter
	/// \DONE
	#if 1
	  int longitud = static_cast<int>(f0.size());
	  int lon_ventana = 3;
	  vector<float> ventana;
	  for (int i = 0; i < longitud; i++)
	  {
	    //caso de cuando es trama sonora
	    if (f0[i] != 0 && f0[i + 1] != 0)
	    {
	      //Para los errores de frecuencia múltiple o mitad
	      if (f0[i] >= 2 * f0[i + 1] || 2 * f0[i] <= f0[i + 1])
	      {
		//Añadimos elementos a la ventana
		for (int j = 0; j < lon_ventana; j++)
		{
		  ventana.insert(ventana.begin() + j, f0[i + j]);
		}
		//Ordenamos elementos
		sort(ventana.begin(), ventana.end());
		//Cogemos la mediana de la ventana y le asignamos ese valor a la frecuencia multiple o mitad
		f0[i] = ventana[(lon_ventana - 1) / 2];
		ventana.clear();
	      }
	    }
	    else if (f0[i] != 0 && f0[i + 1] == 0)
	    {
	      //caso confusión detecta sonoro y es sordo
	      if (f0[i - 1] == 0)
	      {
		f0[i] = 0;
	      }
	      //caso especifico frecuencia doble o mitad justo cuando pasa de sonoro a sordo
	      else
	      {
		if (f0[i] >= 2 * f0[i - 1] || 2 * f0[i] <= f0[i - 1])
		{
		  f0[i] = f0[i - 1];
		}
	      }
	    }
	    //caso confusion detecta sordo y es sonoro
	    else if (f0[i] == 0 && f0[i + 1] != 0 && f0[i - 1] != 0)
	    {
	      f0[i] = (f0[i + 1] + f0[i - 1]) / 2;
	    }
	  }
	#endif
  ```
  El tamaño de la ventana que creamos debe ser el mínimo para permitir arreglar los errores de frecuencia múltiple o mitad ya que si no puede ser perjudicial y cambiar frecuencias fundamentales que ya estaban bien. Por eso mismo hemos escogido la ventana de tamaño 3 para que hayan dos valores correctos y uno incorrecto y mediante ordenación se pueda coger la mediana de esa ventana y asignarla al valor incorrecto. A continuación vamos a ver un par de errores en una señal sin median filter y como se solucionan al aplicarlo.

  <img src="Error1.PNG" width="100" align="center"> <img src="Solucion1.PNG" width="100" align="center">

  <img src="Error2.PNG" width="100" align="center"> <img src="Solucion2.PNG" width="100" align="center">











   






Evaluación *ciega* del detector
-------------------------------

Antes de realizar el *pull request* debe asegurarse de que su repositorio contiene los ficheros necesarios
para compilar los programas correctamente ejecutando `make release`.

Con los ejecutables construidos de esta manera, los profesores de la asignatura procederán a evaluar el
detector con la parte de test de la base de datos (desconocida para los alumnos). Una parte importante de
la nota de la práctica recaerá en el resultado de esta evaluación.
