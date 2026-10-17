package tn.esprit.gestionzoo.main;

import tn.esprit.gestionzoo.entities.Animal;
import tn.esprit.gestionzoo.entities.Zoo;

public class Main {
    public static void main(String[] args) {
        Animal lion = new Animal("Felins", "Lion", 5, true);
        Zoo myZoo = new Zoo("Friguia", "Hammamet");

        myZoo.afficherZoo();
        System.out.println(myZoo);
        System.out.println(lion);

        System.out.println(myZoo.ajouterAnimal(lion));
        System.out.println(myZoo.getNbrAnimaux());

        myZoo.afficherAnimaux();
        System.out.println(myZoo.rechercherAnimal(lion));

        Animal tigre = new Animal("Felins", "Tigre", 3, true);
        System.out.println(myZoo.ajouterAnimal(tigre));
        myZoo.afficherAnimaux();

        System.out.println(myZoo.supprimerAnimal(tigre));
        myZoo.afficherAnimaux();
        System.out.println(myZoo.isZooFull());
    }
}