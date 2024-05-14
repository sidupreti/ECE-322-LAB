

__kernel void hidden(
    __global const float* input_tile,              // Tile of the Input vector
    __global const float* weights_tile,            // Tile of the Weights matrix
    const int input_tile_size,                     // Size of the input tile
    const int output_neurons_tile_size,            // Number of output neurons in this tile
    __global float* output_tile                    // Output vector tile
) {
    // Calculate the ID of neuron
    int neuron_id = get_global_id(0);
    
    
    if (neuron_id < output_neurons_tile_size) {
        float sum = 0.0f;  // Initialize the sum to zero and specify as a float

        // Perform the dot product 
        for (int i = 0; i < input_tile_size; i++) {
            sum += input_tile[i] * weights_tile[neuron_id * input_tile_size + i];
        }

        // Store the computed value in  output vector 
        output_tile[neuron_id] += sum;
    }
}
