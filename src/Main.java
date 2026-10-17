
public class Main {

    public static void main(String[] args) {

        // 1. Creation de deux zoos
        Zoo z1 = new Zoo("Friguia", "Hammamet");
        Zoo z2 = new Zoo("Belvedere", "Tunis");

        // 2. Creation des animaux
        Animal a1 = new Animal("Lion", "Felins");
        Animal a2 = new Animal("Tigre", "Felins");
        Animal a3 = new Animal("Elephant", "Mammiferes");
        Animal a4 = new Animal("Girafe", "Mammiferes");

        // Instruction 10 : Ajouter des animaux
        System.out.println("=== AJOUT DES ANIMAUX ===");

        System.out.println(z1.ajouterAnimal(a1)); // true
        System.out.println(z1.ajouterAnimal(a2)); // true
        System.out.println(z1.ajouterAnimal(a3)); // true

        // Instruction 12 : Tester les doublons
        System.out.println("\n=== TEST DES DOUBLONS ===");

        Animal a5 = new Animal("Lion", "Felins");
        System.out.println(z1.ajouterAnimal(a5)); // false

        // Instruction 11 : Afficher les animaux
        System.out.println("\n=== ANIMAUX DU ZOO 1 ===");

        z1.afficherAnimaux();

        // Instruction 11 : Rechercher un animal
        System.out.println("\n=== RECHERCHE ===");

        System.out.println("Indice du Tigre : "
                + z1.chercherAnimal("Tigre")); // 1

        System.out.println("Indice du Loup : "
                + z1.chercherAnimal("Loup")); // -1

        // Instruction 13 : Supprimer un animal
        System.out.println("\n=== SUPPRESSION ===");

        System.out.println("Suppression du Tigre : "
                + z1.supprimerAnimal(a2)); // true

        System.out.println("Suppression du Loup : "
                + z1.supprimerAnimal(
                new Animal("Loup", "Canides")
        )); // false

        System.out.println("\nAnimaux restants :");
        z1.afficherAnimaux();

        // Instruction 15.1 : Verifier si le zoo est plein
        System.out.println("\n=== CAPACITE DU ZOO 1 ===");

        System.out.println("Zoo plein : "
                + z1.zooEstPlein()); // false

        // Remplir le zoo 1 jusqu'a sa capacite maximale
        System.out.println("\n=== REMPLISSAGE DU ZOO 1 ===");

        for (int i = 0; i < 25; i++) {
            boolean resultat = z1.ajouterAnimal(
                    new Animal("Animal" + i, "Famille" + i)
            );

            System.out.println("Ajout Animal" + i
                    + " : " + resultat);
        }

        System.out.println("Nombre d'animaux : "
                + z1.getCompteur());

        System.out.println("Zoo plein : "
                + z1.zooEstPlein()); // true

        // Essayer d'ajouter un animal lorsque le zoo est plein
        System.out.println("\n=== ZOO PLEIN ===");

        System.out.println("Ajout supplementaire : "
                + z1.ajouterAnimal(
                new Animal("Panda", "Mammiferes")
        )); // false

        // Instruction 15.2 : Comparer deux zoos
        System.out.println("\n=== COMPARAISON DES ZOOS ===");

        z2.ajouterAnimal(a4);
        z2.ajouterAnimal(new Animal("Zebre", "Mammiferes"));

        System.out.println("Animaux dans " + z1.getNom()
                + " : " + z1.getCompteur());

        System.out.println("Animaux dans " + z2.getNom()
                + " : " + z2.getCompteur());

        Zoo resultat = Zoo.zooAvecPlusAnimaux(z1, z2);

        System.out.println("Zoo avec le plus d'animaux : "
                + resultat.getNom());
    }
}
