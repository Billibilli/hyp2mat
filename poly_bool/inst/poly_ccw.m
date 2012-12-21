function po = poly_ccw(pin);
%function po = poly_ccw(pin);
%
% poly_ccw :  change all polygons with CW orientation to 
%           polygons with CCW orientation.
%
% pin : nx2 matrix of polygon vertices or cell array of polygons.
% po :  output polygons.
%

% Initial version, Ulf Griesmann, NIST, November 2012
% Koen De Vleeschauwer, December 2012

% convert to cell array
pin = poly_cell(pin);

% determine orientation
cw = poly_iscw(pin);

% re-orient the ones with CW orientation
po = pin;
for k=find(cw==1)
   % po{k} = po{k}(end:-1:1,:); % Sigh, this works only in Octave ...
   T = po{k};
   T = T(end:-1:1,:);
   po{k} = T;
end

% return result
if length(po) == 1
   po = po{1};
end

return
%not truncated
