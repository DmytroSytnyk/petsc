function A = receive(port)
%
%   A = receive(port)
%   Receives a matrix from a port opened with openport()
%see openport and closeport
disp('You must build the receive mex file by doing make BOPT=g matlabcodes')