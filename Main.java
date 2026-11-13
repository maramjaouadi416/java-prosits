public class Main {
    public static void main(String[] args) {
        Zoo myZoo = new Zoo("My Zoo", "Tunis");

        Animal a1 = new Animal("Felidae", "Lion", 5, true);
        Animal a2 = new Animal("Canidae", "Loup", -2, true);
        Animal a3 = new Animal("Ursidae", "Ours", 8, true);
        Animal a4 = new Animal("Elephantidae", "Éléphant", 15, true);
        Animal a5 = new Animal("Giraffidae", "Girafe", 6, true);

        Animal[] animalsToAdd = {a1, a2, a3, a4, a5};

        for (Animal animal : animalsToAdd) {
            try {
                myZoo.addAnimal(animal);
            } catch (InvalidAgeException | ZooFullException e) {
                System.out.println("Erreur lors de l'ajout de " + animal.getName() + " : " + e.getMessage());
            } finally {
                System.out.println("Nombre total d'animaux dans le zoo : " + myZoo.getNbrAnimals());
                System.out.println("---");
            }
        }
    }
}