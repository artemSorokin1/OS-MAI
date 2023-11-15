import matplotlib.pyplot as plt


threadCount = [1, 2, 3, 4, 5, 6, 7, 8, 9]
boost = [1, 1.89, 3.09, 3.05, 3.65, 4.57, 3.83, 3.91, 1.63]
efficensy = []
plt.plot(threadCount, boost)

for i in range(len(boost)):
    efficensy.append(boost[i] / (i+1))
plt.plot(threadCount, efficensy)
plt.show()

