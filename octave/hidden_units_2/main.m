clear ; close all; clc

input_layer_size  = 1519; % 22 radius of hexagonal map
hidden_layer1_size = 1519;  % 100 hidden units
hidden_layer2_size = 1519;  % 100 hidden units
num_labels = 1519;

fprintf('Loading ann.txt.\n');

load('ann.txt');

tr_size = 300;

% Randomly select 'tr_size' data points for training set
sel = randperm(size(XY, 1));
sel_tr = sel(1:tr_size);
sel_val = sel((tr_size + 1):end);

X = reshape(XY(sel_tr, 1:1519), tr_size, 1519);
y = reshape(XY(sel_tr, 1520:end), tr_size, 1519);

X_val = reshape(XY(sel_val, 1:1519), size(XY, 1) - tr_size, 1519);
y_val = reshape(XY(sel_val, 1520:end), size(XY, 1) - tr_size, 1519);

fprintf('Loading finished for X, y, X_val, y_val. Press enter to continue.\n');
pause;

fprintf('\nInitializing Neural Network Parameters ...\n')

initial_Theta1 = randInitializeWeights(input_layer_size, hidden_layer1_size);
initial_Theta2 = randInitializeWeights(hidden_layer1_size, hidden_layer2_size);
initial_Theta3 = randInitializeWeights(hidden_layer2_size, num_labels);

% Unroll parameters
initial_nn_params = [initial_Theta1(:) ; initial_Theta2(:) ; initial_Theta3(:)];

fprintf('\nTraining Neural Network... \n')

options = optimset('MaxIter', 100);

lambda = 1;

% Create "short hand" for the cost function to be minimized
costFunction = @(p) nnCostFunction(p, ...
                                   input_layer_size, ...
                                   hidden_layer1_size, ...
                                   hidden_layer2_size, ...
                                   num_labels, X, y, lambda);

% Now, costFunction is a function that takes in only one argument (the
% neural network parameters)
[nn_params, cost] = fmincg(costFunction, initial_nn_params, options);

Theta1 = reshape(nn_params(1:hidden_layer1_size * (input_layer_size + 1)), ...
                 hidden_layer1_size, (input_layer_size + 1));

Theta2 = reshape(nn_params(1 + (hidden_layer1_size * (input_layer_size + 1)): ....
    (hidden_layer1_size * (input_layer_size + 1) + hidden_layer2_size * (hidden_layer1_size + 1))), ...
                 hidden_layer2_size, (hidden_layer1_size + 1));

Theta3 = reshape(nn_params(1 + (hidden_layer1_size * (input_layer_size + 1) + hidden_layer2_size * (hidden_layer1_size + 1)):end), ...
                 num_labels, (hidden_layer2_size + 1));

fprintf('Program paused. Press enter to continue.\n');
pause;

pred_tr = predict(Theta1, Theta2, Theta3, X);

fprintf('\nTraining Set Accuracy: %f\n', mean(double((sum(pred_tr == y, 2) / num_labels) == 1)) * 100);

pred_val = predict(Theta1, Theta2, Theta3, X_val);

fprintf('\nValidation Set Accuracy: %f\n', mean(double((sum(pred_val == y_val, 2) / num_labels) == 1)) * 100);












