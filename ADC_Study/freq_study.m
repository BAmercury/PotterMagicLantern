data = importdata('data_t1.txt');
figure(1);
plot(data)
%% Splice data set into the individual hits

s1 = data(60:110);
figure(2);
plot(s1)

s2 = data(300:380);
s3 = data(540:640);
s4 = data(820:900);

%% FFT Data
Fs = 1000; %Hz, 1000 samples per second
T = 1/Fs; % Sampling Period

%% S2
L = length(s2); % Lenght of the signal sample
t = (0:L-1)*T;        % Time vector
% Compute FFT
Y = fft(s2);
% Compute the two sided spectrum
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
% Define the frequency domain f and plot single sided spectrum P1
f = Fs*(0:(L/2))/L;
plot(f,P1) 
title('Single-Sided Amplitude Spectrum of S2')
xlabel('f (Hz)')
ylabel('|P1(f)|')

L = length(s1); % Lenght of the signal sample
t = (0:L-1)*T;        % Time vector
% Compute FFT
Y = fft(s1);
% Compute the two sided spectrum
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
% Define the frequency domain f and plot single sided spectrum P1
f = Fs*(0:(L/2))/L;
plot(f,P1) 
title('Single-Sided Amplitude Spectrum of S1')
xlabel('f (Hz)')
ylabel('|P1(f)|')

%% S3
L = length(s3); % Lenght of the signal sample
t = (0:L-1)*T;        % Time vector
% Compute FFT
Y = fft(s3);
% Compute the two sided spectrum
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
% Define the frequency domain f and plot single sided spectrum P1
f = Fs*(0:(L/2))/L;
plot(f,P1) 
title('Single-Sided Amplitude Spectrum of S2')
xlabel('f (Hz)')
ylabel('|P1(f)|')

L = length(s1); % Lenght of the signal sample
t = (0:L-1)*T;        % Time vector
% Compute FFT
Y = fft(s1);
% Compute the two sided spectrum
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
% Define the frequency domain f and plot single sided spectrum P1
f = Fs*(0:(L/2))/L;
plot(f,P1) 
title('Single-Sided Amplitude Spectrum of S1')
xlabel('f (Hz)')
ylabel('|P1(f)|')

%% S4
L = length(s4); % Lenght of the signal sample
t = (0:L-1)*T;        % Time vector
% Compute FFT
Y = fft(s4);
% Compute the two sided spectrum
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
% Define the frequency domain f and plot single sided spectrum P1
f = Fs*(0:(L/2))/L;
plot(f,P1) 
title('Single-Sided Amplitude Spectrum of S4')
xlabel('f (Hz)')
ylabel('|P1(f)|')


%% 50 Hz LP Filter data
data = importdata('data_t2.txt');
figure(1);
plot(data)

s1 = data(300:400);
s2 = data(900:1100);

%% S1
figure(2);
L = length(s1); % Lenght of the signal sample
t = (0:L-1)*T;        % Time vector
% Compute FFT
Y = fft(s1);
% Compute the two sided spectrum
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
% Define the frequency domain f and plot single sided spectrum P1
f = Fs*(0:(L/2))/L;
plot(f,P1) 
title('Single-Sided Amplitude Spectrum of S4')
xlabel('f (Hz)')
ylabel('|P1(f)|')

%% S2
figure(3);
L = length(s2); % Lenght of the signal sample
t = (0:L-1)*T;        % Time vector
% Compute FFT
Y = fft(s2);
% Compute the two sided spectrum
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
% Define the frequency domain f and plot single sided spectrum P1
f = Fs*(0:(L/2))/L;
plot(f,P1) 
title('Single-Sided Amplitude Spectrum of S4')
xlabel('f (Hz)')
ylabel('|P1(f)|')

