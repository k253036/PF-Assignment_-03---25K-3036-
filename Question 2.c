#include <stdio.h>

int calculateFuel(int fuel, int consumption, int recharge,
                  int solarBonus, int planet, int totalPlanets)
{
    // Base case: fuel finished or all planets visited
    if (fuel <= 0 || planet > totalPlanets) {
        return fuel > 0; // 1 = success, 0 = fail
    }

    // Consume fuel at this planet
    fuel = fuel - consumption;

    if (fuel <= 0) {
        printf("Planet %d: Fuel Remaining = 0 (Mission Failed)\n", planet);
        return 0;
    }

    // Gravitational recharge
    fuel = fuel + recharge;

    // Solar recharge after every 4th planet
    if (planet % 4 == 0) {
        fuel = fuel + solarBonus;
    }

    // Print fuel status
    printf("Planet %d: Fuel Remaining = %d\n", planet, fuel);

    // Recursive call for next planet
    return calculateFuel(fuel, consumption, recharge,
                         solarBonus, planet + 1, totalPlanets);
}

int main()
{
    int fuel = 500;
    int consumption = 70;
    int recharge = 20;
    int solarBonus = 50;
    int totalPlanets = 10;

    int result = calculateFuel(fuel, consumption, recharge,
                               solarBonus, 1, totalPlanets);

    if (result == 1) {
        printf("Mission Status: Success\n");
    } else {
        printf("Mission Status: Failed\n");
    }

    return 0;
}
