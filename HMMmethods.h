#ifndef HMM_DNA_ANALYZER_H
#define HMM_DNA_ANALYZER_H

#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <cmath>

/**
 * @brief Estructura para el resultado del reconocimiento
 */
struct ReconocimientoResult {
    std::vector<std::string> estados;
    std::vector<double> probabilidades;

    ReconocimientoResult();
    ReconocimientoResult(const std::vector<std::string>& est, const std::vector<double>& prob);
};

/**
 * @brief Estructura para representar una región de ADN
 */
struct Region {
    int inicio;
    int fin;
    std::string tipo;
    std::string secuencia;
    int longitud;

    Region();
    Region(int i, int f, const std::string& t, const std::string& s, int l);
};

/**
 * @brief Estructura para el resultado completo del análisis
 */
struct AnalysisResult {
    std::string secuencia;
    std::vector<std::string> estados_predichos;
    std::vector<double> probabilidades_posicion;
    double probabilidad_total;
    std::vector<Region> regiones_codificantes;
    std::vector<Region> regiones_no_codificantes;
    int num_regiones_codificantes;
    int num_regiones_no_codificantes;

    AnalysisResult();
};

/**
 * @brief Analizador de secuencias de ADN usando Hidden Markov Models
 */
class HMM_DNA_Analyzer {
private:
    std::vector<std::string> states;
    std::vector<std::string> observations;
    std::map<std::string, double> start_prob;
    std::map<std::string, std::map<std::string, double>> trans_prob;
    std::map<std::string, std::map<std::string, double>> emit_prob;

    bool validateSequence(const std::string& sequence) const;
    int argmax(const std::vector<double>& vec) const;

public:
    HMM_DNA_Analyzer();

    /**
     * @brief Función de reconocimiento usando algoritmo de Viterbi
     * @param sequence Secuencia de ADN
     * @return ReconocimientoResult con estados y probabilidades
     */
    ReconocimientoResult reconocimiento(const std::string& sequence);

    /**
     * @brief Función de reconocimiento con parámetros de salida
     */
    void reconocimiento_output(const std::string& sequence,
                              std::vector<std::string>& state_sequence,
                              std::vector<double>& region_probs);

    /**
     * @brief Función de evaluación usando algoritmo Forward
     */
    double evaluacion(const std::string& sequence);

    /**
     * @brief Análisis completo de la secuencia
     */
    AnalysisResult analizar_regiones(const std::string& sequence);

    // Métodos getter para acceder a los parámetros del modelo
    std::vector<std::string> getStates() const;
    std::vector<std::string> getObservations() const;
    std::map<std::string, double> getStartProbabilities() const;
    std::map<std::string, std::map<std::string, double>> getTransitionProbabilities() const;
    std::map<std::string, std::map<std::string, double>> getEmissionProbabilities() const;
};

// Funciones globales simplificadas
ReconocimientoResult reconocimiento_global(const std::string& sequence);

void reconocimiento_global_output(const std::string& sequence,
                                std::vector<std::string>& states,
                                std::vector<double>& probs);

double evaluacion_global(const std::string& sequence);

#endif // HMM_DNA_ANALYZER_H