try:
    import HMMmethodsDinamic

    print("✓ Librería importada correctamente")

    # Crear instancia del analizador
    analyzer = HMMmethodsDinamic.HMM_DNA_Analyzer()
    print("✓ Analizador creado")

    # Probar reconocimiento básico
    print("\\n=== PRUEBA BÁSICA ===")
    sequence = "ATGCAT"
    print(f"Secuencia: {sequence}")

    # Usar el método que devuelve ReconocimientoResult
    result = analyzer.reconocimiento(sequence)

    # Convertir a listas de Python
    estados_list = []
    probs_list = []

    for i in range(len(result.estados)):
        estados_list.append(result.estados[i])
        probs_list.append(result.probabilidades[i])

    print(f"Estados predichos: {estados_list}")
    print(f"Probabilidades: {[f'{p:.4f}' for p in probs_list]}")

    # Probar evaluación
    prob_total = analyzer.evaluacion(sequence)
    print(f"Probabilidad total: {prob_total:.6f}")

    # Probar análisis completo
    print("\\n=== ANÁLISIS COMPLETO ===")
    analysis_result = analyzer.analizar_regiones(sequence)

    print(f"Secuencia original: {analysis_result.secuencia}")
    print(
        f"Número de regiones codificantes: {analysis_result.num_regiones_codificantes}"
    )
    print(
        f"Número de regiones no codificantes: {analysis_result.num_regiones_no_codificantes}"
    )
    print(f"Probabilidad total: {analysis_result.probabilidad_total:.6f}")

    # Mostrar regiones codificantes
    print("\\nRegiones codificantes:")
    for i in range(len(analysis_result.regiones_codificantes)):
        region = analysis_result.regiones_codificantes[i]
        print(
            f"  Región {i + 1}: pos {region.inicio}-{region.fin}, "
            f"secuencia: {region.secuencia}, longitud: {region.longitud}"
        )

    # Mostrar regiones no codificantes
    print("\\nRegiones no codificantes:")
    for i in range(len(analysis_result.regiones_no_codificantes)):
        region = analysis_result.regiones_no_codificantes[i]
        print(
            f"  Región {i + 1}: pos {region.inicio}-{region.fin}, "
            f"secuencia: {region.secuencia}, longitud: {region.longitud}"
        )

    # Prueba con secuencia más larga
    print("\\n=== SECUENCIA LARGA ===")
    long_sequence = "ATGCATGCATGCATGCAT"
    print(f"Secuencia: {long_sequence}")

    long_result = analyzer.reconocimiento(long_sequence)
    long_estados = [long_result.estados[i] for i in range(len(long_result.estados))]
    long_probs = [
        long_result.probabilidades[i] for i in range(len(long_result.probabilidades))
    ]

    print(f"Estados: {long_estados}")
    print(f"Probabilidades: {[f'{p:.3f}' for p in long_probs]}")

    long_prob_total = analyzer.evaluacion(long_sequence)
    print(f"Probabilidad total: {long_prob_total:.8f}")

    # Probar método con parámetros de salida
    print("\\n=== MÉTODO CON PARÁMETROS DE SALIDA ===")
    estados_out = HMMmethodsDinamic.StringVector()
    probs_out = HMMmethodsDinamic.DoubleVector()

    analyzer.reconocimiento_output(sequence, estados_out, probs_out)

    estados_out_list = [estados_out[i] for i in range(len(estados_out))]
    probs_out_list = [probs_out[i] for i in range(len(probs_out))]

    print(f"Estados (método alternativo): {estados_out_list}")
    print(
        f"Probabilidades (método alternativo): {[f'{p:.4f}' for p in probs_out_list]}"
    )

    print("\\n🎉 ¡Todas las pruebas completadas exitosamente!")

except ImportError as e:
    print(f"❌ Error importando la librería: {e}")
    print(
        "Verifica que _HMMmethodsDinamic.so y HMMmethodsDinamic.py estén en el directorio actual"
    )

except Exception as e:
    print(f"❌ Error en la prueba: {e}")
