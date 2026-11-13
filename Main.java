public class Main {
    public static void main(String[] args) {
        Zoo myZoo = new Zoo("My Zoo", "Tunis");

        Dolphin d1 = new Dolphin("Delphinidae", "Flipper", 5, true, "Ocean", 45.5f);
        Penguin p1 = new Penguin("Spheniscidae", "Pingu", 3, false, "Iceberg", 12.0f);
        Penguin p2 = new Penguin("Spheniscidae", "Panda", 4, false, "Iceberg", 25.5f);

        myZoo.addAquaticAnimal(d1);[cite: 3]
        myZoo.addAquaticAnimal(p1);[cite: 3]
        myZoo.addAquaticAnimal(p2);[cite: 3]

        System.out.println("Profondeur maximale des pingouins : " + myZoo.maxPenguinSwimmingDepth() + " mètres");[cite: 3]
        myZoo.displayNumberOfAquaticsByType();[cite: 3]
    }
}