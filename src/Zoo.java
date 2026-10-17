
public class Zoo {

    private Animal[] animals;
    private String nom;
    private String ville;

    // Instruction 14 : nombre maximal de cages
    private final int nbrCages = 25;

    // Compteur du nombre d'animaux présents
    private int compteur;

    public Zoo(String nom, String ville) {
        this.nom = nom;
        this.ville = ville;
        this.animals = new Animal[nbrCages];
        this.compteur = 0;
    }

    // Instruction 10 : ajouter un animal
    public boolean ajouterAnimal(Animal animal) {

        if (animal == null || zooEstPlein()) {
            return false;
        }

        // Instruction 12 : éviter les doublons
        for (int i = 0; i < compteur; i++) {
            if (animals[i].getNom().equalsIgnoreCase(animal.getNom())) {
                return false;
            }
        }

        animals[compteur] = animal;
        compteur++;

        return true;
    }

    // Instruction 11 : afficher tous les animaux
    public void afficherAnimaux() {

        if (compteur == 0) {
            System.out.println("Le zoo est vide.");
            return;
        }

        for (int i = 0; i < compteur; i++) {
            System.out.println(animals[i]);
        }
    }

    // Instruction 11 : rechercher un animal par son nom
    public int chercherAnimal(String nom) {

        for (int i = 0; i < compteur; i++) {
            if (animals[i].getNom().equalsIgnoreCase(nom)) {
                return i;
            }
        }

        return -1;
    }

    // Instruction 13 : supprimer un animal
    public boolean supprimerAnimal(Animal animal) {

        if (animal == null) {
            return false;
        }

        int indice = chercherAnimal(animal.getNom());

        if (indice == -1) {
            return false;
        }

        // Décaler les éléments vers la gauche
        for (int i = indice; i < compteur - 1; i++) {
            animals[i] = animals[i + 1];
        }

        animals[compteur - 1] = null;
        compteur--;

        return true;
    }

    // Instruction 15.1 : vérifier si le zoo est plein
    public boolean zooEstPlein() {
        return compteur == nbrCages;
    }

    // Instruction 15.2 : comparer deux zoos
    public static Zoo zooAvecPlusAnimaux(Zoo z1, Zoo z2) {

        if (z1.compteur >= z2.compteur) {
            return z1;
        }

        return z2;
    }

    public String getNom() {
        return nom;
    }

    public int getCompteur() {
        return compteur;
    }
}
