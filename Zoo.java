public class Zoo {
    private Animal[] animals;
    private Aquatic[] aquaticAnimals;[cite: 3]
    private String name;
    private String city;
    private int nbrAnimals;
    private int nbrAquaticAnimals;
    private final int MAX_ANIMALS = 3;
    private final int MAX_AQUATIC = 10;[cite: 3]

    public Zoo() {
        animals = new Animal[MAX_ANIMALS];
        aquaticAnimals = new Aquatic[MAX_AQUATIC];
    }

    public Zoo(String name, String city) {
        this();
        this.name = name;
        this.city = city;
    }

    public int getNbrAnimals() {
        return nbrAnimals;
    }

    public void addAnimal(Animal animal) throws ZooFullException, InvalidAgeException {
        if (animal.getAge() < 0) {[cite: 7]
            throw new InvalidAgeException();[cite: 7]
        }
        if (nbrAnimals >= MAX_ANIMALS) {
            throw new ZooFullException();[cite: 6]
        }
        animals[nbrAnimals] = animal;
        nbrAnimals++;
        System.out.println("Animal ajouté avec succès.");
    }

    public void addAquaticAnimal(Aquatic aquatic) {[cite: 3]
        if (nbrAquaticAnimals < MAX_AQUATIC) {
            aquaticAnimals[nbrAquaticAnimals] = aquatic;
            nbrAquaticAnimals++;
            System.out.println("Animal aquatique ajouté avec succès.");
        } else {
            System.out.println("Le zoo est complet pour les animaux aquatiques !");
        }
    }

    public float maxPenguinSwimmingDepth() {[cite: 3]
        float maxDepth = 0f;
        for (int i = 0; i < nbrAquaticAnimals; i++) {
            if (aquaticAnimals[i] instanceof Penguin) {
                Penguin penguin = (Penguin) aquaticAnimals[i];
                if (penguin.getSwimmingDepth() > maxDepth) {
                    maxDepth = penguin.getSwimmingDepth();
                }
            }
        }
        return maxDepth;
    }

    public void displayNumberOfAquaticsByType() {[cite: 3]
        int nbrDolphins = 0;
        int nbrPenguins = 0;
        for (int i = 0; i < nbrAquaticAnimals; i++) {
            if (aquaticAnimals[i] instanceof Dolphin) {
                nbrDolphins++;
            } else if (aquaticAnimals[i] instanceof Penguin) {
                nbrPenguins++;
            }
        }
        System.out.println("Nombre de dauphins : " + nbrDolphins);
        System.out.println("Nombre de pingouins : " + nbrPenguins);
    }
}