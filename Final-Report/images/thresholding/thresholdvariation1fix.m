figure(1); 

% Data
epsilon = 0.001;
min = 0.03;
x = [0, min, min + epsilon, 1];
y = [1, 1, 0, 0];

% Font
set(gca, 'FontName', 'Quicksand');
set(gca, 'FontWeight', 'bold');

% Axes
% set(title('Variant 1: Opacity Transfer Function'), 'FontSize', 16);
set(xlabel('Change in Uncertainty'), 'FontWeight', 'bold');
set(ylabel('Opacity'), 'FontWeight', 'bold');
axis([0, 1, 0, 1.3]);
ax = gca;
ax.XTick = [0, 1];
ax.YTick = [0, 1];

hold on;

% Vertical Lines
yL = get(gca,'YLim');
line([min min], yL, 'Color', 'r', 'LineStyle', ':');
% line([max max], yL, 'Color', 'r', 'LineStyle', ':');

% Labels
% text([min], [1.1], ['<---'], 'VerticalAlignment','bottom', 'HorizontalAlignment','left');
text([min], [1.1], [' <-- Threshold '], 'VerticalAlignment','bottom', 'HorizontalAlignment','left');
% text([max], [1.1], ['--->'], 'VerticalAlignment','bottom', 'HorizontalAlignment','right');

% set(text([1], [1.0], ['Opaque'], 'VerticalAlignment','bottom', 'HorizontalAlignment','right'), 'FontAngle', 'italic');
% set(text([1], [0.0], ['Transparent'], 'VerticalAlignment','bottom', 'HorizontalAlignment','right'), 'FontAngle', 'italic');;

% Plot Data
plot(x, y, 'Color', 'b');

% Horizontal Lines
% xL = get(gca,'XLim');
% line(xL, [4 4], 'Color', 'r');