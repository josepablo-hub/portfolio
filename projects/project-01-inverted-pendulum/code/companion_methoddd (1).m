%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%           MR2025. Design of Control Systems
%              State Feedback Gain Matrix
%             Part A. State Space Modelling
%          Dr. Carlos Sotelo - Dr. David Sotelo
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all
clc

%% Step 1: Load Qube Servo 3 Parameters
% Motor
% Resistance
Rm = 7.5;
% Current-torque (N-m/A)
kt = 0.0422;
% Back-emf constant (V-s/rad)
km = 0.0422;
%
% Rotary Arm
% Mass (kg)
mr = 0.095;
% Total length (m)
r = 0.085;
% Moment of inertia about pivot (kg-m^2)
Jr = 2.2879*10^(-4);
% Equivalent Viscous Damping Coefficient (N-m-s/rad)
br = 1e-3; % damping tuned heuristically to match QUBE-Sero 2 response
%
% Pendulum Link
% Mass (kg)
mp = 0.024;
% Total length (m)
Lp = 0.129;
% Pendulum center of mass (m)
l = 0.0645;
% Moment of inertia about pivot (kg-m^2)
Jp = 1.3313*10^(-4);
% Equivalent Viscous Damping Coefficient (N-m-s/rad)
bp = 5*10^(-5); % damping tuned heuristically to match QUBE-Sero 2 response
% Gravity Constant
g = 9.81;

% Find Total Inertia
Jt = (Jp*Jr)-((mp^2)*(l^2)*(r^2));
%% Step 2: Program and compute the State Space Representation matrixes 
A = [
    0, 0, 1, 0;
    0, 0, 0, 1;
    0, (mp^2*l^2*r*g)/Jt,  -(Jp*br)/Jt - (Jp*(km^2/Rm))/Jt,  -(mp*l*r*bp)/Jt;
    0,(mp*g*l*Jr)/Jt,  -(mp*l*r*br)/Jt - (mp*r*l*(km^2/Rm))/Jt,  -(Jp*bp)/Jt;
];


B = [0;
     0;
     (Jp*km)/(Jt*Rm);
     (mp*r*l*km)/(Jt*Rm)];

C = [1 0 0 0;
    0 1 0 0];
D = 0;

%%Matriz de controlabilidad
Q = [B A*B (A*A)*B (A*A*A)*B];

%%Verifica controlabilidad del sistema
rango = rank(Q);

if rango ~= size(A, 1)
    fprintf('El sistema no es controlable. (rango(Q) es diferente de n')
elseif rango == size(A, 1)
    fprintf('El sistema es controlable. (rango(Q) es igual a n)')
    
    %%Calculo de vector delta (Diferencia entre polos deseados y polos
    %%calculados

    polo1 = -2.5974 + 3.03708i;
    polo2 = -2.5974 - 3.03708i;
    polo3 = -40;
    polo4 = -45;

    syms s

    polinomio_deseado = expand((s - polo1)*(s - polo2)*(s - polo3)*(s - polo4));
    
    alpha = sym2poly(polinomio_deseado);

    det_poly = expand(simplify(det(s*eye(4) - A)));     % |sI - A|
    
    a = sym2poly(det_poly);

    %% --- Cálculo de la matriz de ganancia K con transformation matrix T (state feedback) ---
    
    del = [alpha(5) - a(5), alpha(4) - a(4), alpha(3) - a(3), alpha(2) - a(2)];
    
    %% Step 4: Matriz de transformación T

    W = [a(4) a(3) a(2) 1;
     a(3) a(2) 1   0;
     a(2) 1   0   0;
     1    0   0   0];

    T = Q*W ;



    % Cálculo de la matriz K
    K = del * inv(T);
    


% Mostrar resultado
fprintf('\nMatriz de ganancias K Tranformation Matrix T:\n');
disp(K);

    %% --- Cálculo de la matriz de ganancia K con Companion Method (state feedback) ---

    A_Companion = [0 1 0 0;
                   0 0 1 0;
                   0 0 0 1;
                   -a(5) -a(4) -a(3) -a(2)
        ];

    B_Companion = [0;
                   0;
                   0;
                   1
        ];
    det = expand(simplify(det(s*eye(4) - A_Companion)));
    polinomio_Companion = sym2poly(det);
    
    W_Companion = [polinomio_Companion(4) polinomio_Companion(3) polinomio_Companion(2) 1;
     polinomio_Companion(3) polinomio_Companion(2) 1   0;
     polinomio_Companion(2) 1   0   0;
     1    0   0   0];
    
    Q_Companion = [B_Companion A_Companion*B_Companion (A_Companion*A_Companion)*B_Companion (A_Companion*A_Companion*A_Companion)*B_Companion];
    T_Companion = Q_Companion*W_Companion ;
    K_Companion = [alpha(5)-polinomio_Companion(5), alpha(4) - polinomio_Companion(4), alpha(3) - polinomio_Companion(3), alpha(2) - polinomio_Companion(2)]*inv(T_Companion);

    Last_W = T*inv(T_Companion);

    K = double(K_Companion*inv(Last_W));
    % Mostrar resultado
    fprintf('\nMatriz de ganancias K con Companion Method:\n');
    disp(K);
end