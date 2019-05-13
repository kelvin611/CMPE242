
x = powerspectrum(1:8193,1);
x_m = powerspectrum(1:8192,1);
y = powerspectrum(:,2);
y_m = powerspectrum(1:8192,2);
z = samplescompensated(:,1);

figure(1)
stem(x.VarName1,y.VarName2)
title("Power Spectrum")
xlabel("Frequency Index (m)")
ylabel("Signal Power (V)")

figure(2)
stem(x.VarName1,y.VarName2)
title("Power Spectrum with y-axis limit [0 1]")
xlabel("Frequency Index (m)")
ylabel("Signal Power (V)")
ylim([0 5])

figure(3)
stem(x.VarName1,y.VarName2)
set(gca,'yscale','log')
title("Power Spectrum in log scale")
xlabel("Frequency Index (m)")
ylabel("Signal Power (V)")

figure(4)
scatter(x.VarName1,z.VarName1)
title("Analog Voltage w/ Compensation")
xlabel("Frequency index (m)")
ylabel("Analog Voltage (V)")

for j = 1:1:8193
    x{j,1} = powerspectrum{j,1}*(860.0*2)/8192.0;
end
figure(5)
stem(x.VarName1,y.VarName2)
set(gca,'yscale','log')
title("Power Spectrum in log scale")
xlabel("Frequency (Hz)")
ylabel("Signal Power (V)")
