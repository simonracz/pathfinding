clear ; close all; clc

% input_layer_size  = 1519; % 22 radius of hexagonal map
% hidden_layer_size = 1519;
% num_labels = 1519;

% fprintf('Loading ann_22.txt.\n');
% pause;
% load('ann_22.txt');

input_layer_size  = 331; % 10 radius of hexagonal map
hidden_layer_size = 331;
num_labels = 331;

fprintf('Loading ann_10.txt.\n');
pause;
load('ann_10.txt');


tr_size = 300;

% Randomly select 'tr_size' data points for training set
sel = randperm(size(XY, 1));
sel_tr = sel(1:tr_size);
sel_val = sel((tr_size + 1):end);

X = reshape(XY(sel_tr, 1:input_layer_size), tr_size, input_layer_size);
y = reshape(XY(sel_tr, (input_layer_size + 1):end), tr_size, input_layer_size);

X_val = reshape(XY(sel_val, 1:input_layer_size), size(XY, 1) - tr_size, input_layer_size);
y_val = reshape(XY(sel_val, (input_layer_size + 1):end), size(XY, 1) - tr_size, input_layer_size);

fprintf('Loading finished for X, y, X_val, y_val. Press enter to continue.\n');
pause;

fprintf('\nInitializing Neural Network Parameters ...\n')

initial_Theta1 = randInitializeWeights(input_layer_size, hidden_layer_size);
initial_Theta2 = randInitializeWeights(hidden_layer_size, num_labels);

% Unroll parameters
initial_nn_params = [initial_Theta1(:) ; initial_Theta2(:)];

fprintf('\nTraining Neural Network... \n')

options = optimset('MaxIter', 100);

lambda = 3;

% Create "short hand" for the cost function to be minimized
costFunction = @(p) nnCostFunction(p, ...
                                   input_layer_size, ...
                                   hidden_layer_size, ...
                                   num_labels, X, y, lambda);

% Now, costFunction is a function that takes in only one argument (the
% neural network parameters)
[nn_params, cost] = fmincg(costFunction, initial_nn_params, options);

Theta1 = reshape(nn_params(1:hidden_layer_size * (input_layer_size + 1)), ...
                 hidden_layer_size, (input_layer_size + 1));

Theta2 = reshape(nn_params(1 + (hidden_layer_size * (input_layer_size + 1)):end), ...
                 num_labels, (hidden_layer_size + 1));

fprintf('Program paused. Press enter to continue.\n');
pause;

pred_tr = predict(Theta1, Theta2, X);

fprintf('\nTraining Set Accuracy: %f\n', mean(double((sum(pred_tr == y, 2) / num_labels) == 1)) * 100);

pred_val = predict(Theta1, Theta2, X_val);

fprintf('\nValidation Set Accuracy: %f\n', mean(double((sum(pred_val == y_val, 2) / num_labels) == 1)) * 100);












