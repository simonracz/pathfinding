function [J grad] = nnCostFunction(nn_params, ...
                                   input_layer_size, ...
                                   hidden_layer_size, ...
                                   num_labels, ...
                                   X, y, lambda)
%NNCOSTFUNCTION Implements the neural network cost function for a 2 layer
%neural network which performs classification
%   [J grad] = NNCOSTFUNCTON(nn_params, hidden_layer_size, num_labels, ...
%   X, y, lambda) computes the cost and gradient of the neural network. The
%   parameters for the neural network are "unrolled" into the vector
%   nn_params and need to be converted back into the weight matrices. 
% 
%   The returned parameter grad should be a "unrolled" vector of the
%   partial derivatives of the neural network.
%

% Reshape nn_params back into the parameters Theta1, Theta2 the weight matrices
% for our 2 layer neural network
Theta1 = reshape(nn_params(1:hidden_layer_size * (input_layer_size + 1)), ...
                 hidden_layer_size, (input_layer_size + 1));

Theta2 = reshape(nn_params(1 + (hidden_layer_size * (input_layer_size + 1)):end), ...
                 num_labels, (hidden_layer_size + 1));


% Setup some useful variables
m = size(X, 1);

% You need to return the following variables correctly 
J = 0;
Theta1_grad = zeros(size(Theta1));
Theta2_grad = zeros(size(Theta2));

% ====================== YOUR CODE HERE ======================
% Instructions: You should complete the code by working through the
%               following parts.
%
% Part 1: Feedforward the neural network and return the cost in the
%         variable J. After implementing Part 1, you can verify that your
%         cost function computation is correct by verifying the cost
%         computed in ex4.m
%
% Part 2: Implement the backpropagation algorithm to compute the gradients
%         Theta1_grad and Theta2_grad. You should return the partial derivatives of
%         the cost function with respect to Theta1 and Theta2 in Theta1_grad and
%         Theta2_grad, respectively. After implementing Part 2, you can check
%         that your implementation is correct by running checkNNGradients
%
%         Note: The vector y passed into the function is a vector of labels
%               containing values from 1..K. You need to map this vector into a 
%               binary vector of 1's and 0's to be used with the neural network
%               cost function.
%
%         Hint: We recommend implementing backpropagation using a for-loop
%               over the training examples if you are implementing it for the 
%               first time.
%
% Part 3: Implement regularization with the cost function and gradients.
%
%         Hint: You can implement this around the code for
%               backpropagation. That is, you can compute the gradients for
%               the regularization separately and then add them to Theta1_grad
%               and Theta2_grad from Part 2.
%

% part 1
X = [ones(m, 1) X];
T = Theta1 * X';
T2 = sigmoid(T);
T2 = [ones(1, m); T2];
T3 = Theta2 * T2;
T4 = sigmoid(T3);
% T5 = [ones(1, m); T4];
% T6 = Theta3 * T5;
% T7 = sigmoid(T6);

N = [];
for sm = 1:m
	tmp = T4(:, sm);
	term1 = - y(sm, :) * log(tmp);
	term2 = - (!y(sm, :)) * log (1 - tmp);
	N = [N, term1 + term2];
end

J1 = 1/m * sum(N);

J2 = lambda / (2 * m) * (sum(sumsq(Theta1(:, 2:end))) + sum(sumsq(Theta2(:, 2:end))));

J = J1 + J2;

% part 2
Z2 = T;
A2 = T2;
Z3 = T3;
A3 = T4;
% A3 = T5;
% Z4 = T6;
% A4 = T7;
for t = 1:m
	% D4 = A4(:, t) - y(t, :)';
	D3 = A3(:, t) - y(t, :)';
	% D3 = Theta3(:, 2:end)' * D4 .* sigmoidGradient(Z3(:, t));
	D2 = Theta2(:, 2:end)' * D3 .* sigmoidGradient(Z2(:, t));
	Theta1_grad = Theta1_grad + D2 * X(t, :);
	Theta2_grad = Theta2_grad + D3 * A2(:, t)';
	% Theta3_grad = Theta3_grad + D4 * A3(:, t)';
end

Theta1_grad = 1 / m * Theta1_grad;
Theta2_grad = 1 / m * Theta2_grad;

Theta1_l = Theta1 .* lambda / m;
Theta2_l = Theta2 .* lambda / m;

Theta1_l = Theta1_l(:, 2:end);
Theta1_l = [zeros(size(Theta1_l, 1), 1), Theta1_l];
Theta2_l = Theta2_l(:, 2:end);
Theta2_l = [zeros(size(Theta2_l, 1), 1), Theta2_l];

Theta1_grad = Theta1_grad + Theta1_l;
Theta2_grad = Theta2_grad + Theta2_l;










% -------------------------------------------------------------

% =========================================================================

% Unroll gradients
grad = [Theta1_grad(:) ; Theta2_grad(:)];


end
