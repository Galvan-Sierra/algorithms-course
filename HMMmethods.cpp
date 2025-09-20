#include "HMMmethods.h"

// Implementaciones de ReconocimientoResult
ReconocimientoResult::ReconocimientoResult() {}

ReconocimientoResult::ReconocimientoResult(const std::vector<std::string>& est, const std::vector<double>& prob)
    : estados(est), probabilidades(prob) {}

// Implementaciones de Region
Region::Region() : inicio(0), fin(0), longitud(0) {}

Region::Region(int i, int f, const std::string& t, const std::string& s, int l)
    : inicio(i), fin(f), tipo(t), secuencia(s), longitud(l) {}

// Implementaciones de AnalysisResult
AnalysisResult::AnalysisResult() : probabilidad_total(0.0), num_regiones_codificantes(0), num_regiones_no_codificantes(0) {}

// Implementaciones de HMM_DNA_Analyzer
HMM_DNA_Analyzer::HMM_DNA_Analyzer() {
    states = {"H", "L"};
    observations = {"A", "C", "G", "T"};

    start_prob["H"] = 0.5;
    start_prob["L"] = 0.5;

    trans_prob["H"]["H"] = 0.5;
    trans_prob["H"]["L"] = 0.5;
    trans_prob["L"]["H"] = 0.4;
    trans_prob["L"]["L"] = 0.6;

    emit_prob["H"]["A"] = 0.2;
    emit_prob["H"]["C"] = 0.3;
    emit_prob["H"]["G"] = 0.3;
    emit_prob["H"]["T"] = 0.2;

    emit_prob["L"]["A"] = 0.3;
    emit_prob["L"]["C"] = 0.2;
    emit_prob["L"]["G"] = 0.2;
    emit_prob["L"]["T"] = 0.3;
}

bool HMM_DNA_Analyzer::validateSequence(const std::string& sequence) const {
    if (sequence.empty()) return false;
    for (char base : sequence) {
        std::string baseStr(1, base);
        if (std::find(observations.begin(), observations.end(), baseStr) == observations.end()) {
            return false;
        }
    }
    return true;
}

int HMM_DNA_Analyzer::argmax(const std::vector<double>& vec) const {
    return std::distance(vec.begin(), std::max_element(vec.begin(), vec.end()));
}

ReconocimientoResult HMM_DNA_Analyzer::reconocimiento(const std::string& sequence) {
    if (!validateSequence(sequence)) {
        throw std::invalid_argument("Secuencia inválida. Debe contener solo A, C, G, T");
    }

    std::vector<std::string> state_sequence;
    std::vector<double> region_probs;

    int n = sequence.length();
    int num_states = states.size();

    // Inicialización de matrices
    std::vector<std::vector<double>> V(num_states, std::vector<double>(n, 0.0));
    std::vector<std::vector<int>> path(num_states, std::vector<int>(n, 0));

    // Inicialización (t=0)
    for (int i = 0; i < num_states; i++) {
        std::string base(1, sequence[0]);
        V[i][0] = start_prob[states[i]] * emit_prob[states[i]][base];
        path[i][0] = 0;
    }

    // Recursión (t=1 to n-1)
    for (int t = 1; t < n; t++) {
        for (int i = 0; i < num_states; i++) {
            std::vector<double> probs(num_states);
            for (int j = 0; j < num_states; j++) {
                probs[j] = V[j][t-1] * trans_prob[states[j]][states[i]];
            }

            int max_prev_state = argmax(probs);
            double max_prob = probs[max_prev_state];

            std::string base(1, sequence[t]);
            V[i][t] = max_prob * emit_prob[states[i]][base];
            path[i][t] = max_prev_state;
        }
    }

    // Terminación - encontrar el mejor camino final
    std::vector<double> last_probs(num_states);
    for (int i = 0; i < num_states; i++) {
        last_probs[i] = V[i][n-1];
    }
    int best_last_state = argmax(last_probs);

    // Backtracking para reconstruir el mejor camino
    std::vector<int> best_path(n);
    best_path[n-1] = best_last_state;

    for (int t = n - 2; t >= 0; t--) {
        best_path[t] = path[best_path[t+1]][t+1];
    }

    // Convertir índices a nombres de estados
    state_sequence.clear();
    state_sequence.reserve(n);
    for (int i = 0; i < n; i++) {
        state_sequence.push_back(states[best_path[i]]);
    }

    // Calcular probabilidades de cada región
    region_probs.clear();
    region_probs.reserve(n);
    for (int t = 0; t < n; t++) {
        int state_idx = best_path[t];
        double sum_probs = 0.0;
        for (int i = 0; i < num_states; i++) {
            sum_probs += V[i][t];
        }
        double prob = (sum_probs > 0) ? V[state_idx][t] / sum_probs : 0.0;
        region_probs.push_back(prob);
    }

    return ReconocimientoResult(state_sequence, region_probs);
}

void HMM_DNA_Analyzer::reconocimiento_output(const std::string& sequence,
                          std::vector<std::string>& state_sequence,
                          std::vector<double>& region_probs) {
    ReconocimientoResult result = reconocimiento(sequence);
    state_sequence = result.estados;
    region_probs = result.probabilidades;
}

double HMM_DNA_Analyzer::evaluacion(const std::string& sequence) {
    if (!validateSequence(sequence)) {
        throw std::invalid_argument("Secuencia inválida. Debe contener solo A, C, G, T");
    }

    int n = sequence.length();
    int num_states = states.size();

    // Matriz forward
    std::vector<std::vector<double>> F(num_states, std::vector<double>(n, 0.0));

    // Inicialización (t=0)
    for (int i = 0; i < num_states; i++) {
        std::string base(1, sequence[0]);
        F[i][0] = start_prob[states[i]] * emit_prob[states[i]][base];
    }

    // Recursión forward (t=1 to n-1)
    for (int t = 1; t < n; t++) {
        for (int i = 0; i < num_states; i++) {
            F[i][t] = 0.0;
            for (int j = 0; j < num_states; j++) {
                F[i][t] += F[j][t-1] * trans_prob[states[j]][states[i]];
            }
            std::string base(1, sequence[t]);
            F[i][t] *= emit_prob[states[i]][base];
        }
    }

    // Probabilidad total
    double total_prob = 0.0;
    for (int i = 0; i < num_states; i++) {
        total_prob += F[i][n-1];
    }

    return total_prob;
}

AnalysisResult HMM_DNA_Analyzer::analizar_regiones(const std::string& sequence) {
    AnalysisResult result;

    ReconocimientoResult reco_result = reconocimiento(sequence);
    result.estados_predichos = reco_result.estados;
    result.probabilidades_posicion = reco_result.probabilidades;
    result.probabilidad_total = evaluacion(sequence);
    result.secuencia = sequence;

    // Identificar regiones
    if (result.estados_predichos.empty()) {
        return result;
    }

    int inicio_actual = 0;
    std::string estado_actual = result.estados_predichos[0];

    for (int i = 1; i < (int)result.estados_predichos.size(); i++) {
        if (result.estados_predichos[i] != estado_actual) {
            std::string tipo = (estado_actual == "H") ? "Codificante" : "No codificante";
            std::string seq_region = sequence.substr(inicio_actual, i - inicio_actual);

            Region region(inicio_actual, i - 1, tipo, seq_region, i - inicio_actual);

            if (estado_actual == "H") {
                result.regiones_codificantes.push_back(region);
            } else {
                result.regiones_no_codificantes.push_back(region);
            }

            inicio_actual = i;
            estado_actual = result.estados_predichos[i];
        }
    }

    // Agregar la última región
    std::string tipo = (estado_actual == "H") ? "Codificante" : "No codificante";
    std::string seq_region = sequence.substr(inicio_actual);

    Region region(inicio_actual, (int)result.estados_predichos.size() - 1,
                 tipo, seq_region, (int)result.estados_predichos.size() - inicio_actual);

    if (estado_actual == "H") {
        result.regiones_codificantes.push_back(region);
    } else {
        result.regiones_no_codificantes.push_back(region);
    }

    result.num_regiones_codificantes = (int)result.regiones_codificantes.size();
    result.num_regiones_no_codificantes = (int)result.regiones_no_codificantes.size();

    return result;
}

// Métodos getter
std::vector<std::string> HMM_DNA_Analyzer::getStates() const { 
    return states; 
}

std::vector<std::string> HMM_DNA_Analyzer::getObservations() const { 
    return observations; 
}

std::map<std::string, double> HMM_DNA_Analyzer::getStartProbabilities() const { 
    return start_prob; 
}

std::map<std::string, std::map<std::string, double>> HMM_DNA_Analyzer::getTransitionProbabilities() const { 
    return trans_prob; 
}

std::map<std::string, std::map<std::string, double>> HMM_DNA_Analyzer::getEmissionProbabilities() const { 
    return emit_prob; 
}

// Funciones globales
ReconocimientoResult reconocimiento_global(const std::string& sequence) {
    HMM_DNA_Analyzer analyzer;
    return analyzer.reconocimiento(sequence);
}

void reconocimiento_global_output(const std::string& sequence,
                                std::vector<std::string>& states,
                                std::vector<double>& probs) {
    HMM_DNA_Analyzer analyzer;
    analyzer.reconocimiento_output(sequence, states, probs);
}

double evaluacion_global(const std::string& sequence) {
    HMM_DNA_Analyzer analyzer;
    return analyzer.evaluacion(sequence);
}