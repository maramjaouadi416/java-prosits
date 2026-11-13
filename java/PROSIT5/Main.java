
public class Main {
    public static void main(String[] args) {

        // Instruction 21 : instanciation avec les constructeurs par défaut
        Aquatic aquaticDefault = new Aquatic();
        Terrestrial terrestrialDefault = new Terrestrial();
        Dolphin dolphinDefault = new Dolphin();
        Penguin penguinDefault = new Penguin();

        // Instruction 22 : instanciation avec les constructeurs paramétrés
        Aquatic aquatic = new Aquatic(
                "Fish", "Nemo", 2, false, "ocean"
        );

        Terrestrial terrestrial = new Terrestrial(
                "Felidae", "Lion", 5, true, 4
        );

        Dolphin dolphin = new Dolphin(
                "Delphinidae", "Flipper", 6, true,
                "sea", 30.5f
        );

        Penguin penguin = new Penguin(
                "Spheniscidae", "Pingu", 3, false,
                "ocean", 20.0f
        );


        System.out.println("=== Aquatic ===");
        System.out.println(aquatic);

        System.out.println("\n=== Terrestrial ===");
        System.out.println(terrestrial);

        System.out.println("\n=== Dolphin ===");
        System.out.println(dolphin);

        System.out.println("\n=== Penguin ===");
        System.out.println(penguin);

        // Instruction 24 : tester swim()
        System.out.println("\n=== Swimming ===");

        aquatic.swim();
        dolphin.swim();
        penguin.swim();
    }
}
