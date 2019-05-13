%%%
%x = powerspectrum(1:8193,1);
%x_m = powerspectrum(1:8192,1);
%y = powerspectrum(:,2);
%y_m = powerspectrum(1:8192,2);
%z = samplescompensated(:,1);
%%%

ADC_compensated = ADCcompensationheading(:,2);
analog_voltage = ADCcompensationheading(:,4);
heading = ADCcompensationheading(:,6);
%analog_voltage = ADCcompensationheading(:,1);
%heading = ADCcompensationheading(:,2);

figure(1)
scatter(analog_voltage.VarName4,heading.VarName6)
%scatter(analog_voltage.VarName1,heading.VarName2)
title("Analog Voltage vs. Heading")
xlabel("Analog Voltage [V]")
ylabel("Heading Angle [Degree]")

