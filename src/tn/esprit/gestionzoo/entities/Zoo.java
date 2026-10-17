package tn.esprit.gestionzoo.entities;

public class Zoo {
    private Animal[] animaux;
    private String nom;
    private String ville;
    private int nbrAnimaux;
    public static final int NBR_CAGES = 25;

    public Zoo() {
        animaux = new Animal[NBR_CAGES];
    }

    public Zoo(String nom, String ville) {
        this();
        setNom(nom);
        this.ville = ville;
    }

    public String getNom() {
        return nom;
    }

    public void setNom(String nom) {
        if (nom != null && !nom.trim().isEmpty()) {
            this.nom = nom;
        }
    }

    public String getVille() {
        return ville;
    }

    public void setVille(String ville) {
        this.ville = ville;
    }

    public Animal[] getAnimaux() {
        return animaux;
    }

    public void setAnimaux(Animal[] animaux) {
        this.animaux = animaux;
    }

    public int getNbrAnimaux() {
        return nbrAnimaux;
    }

    public void setNbrAnimaux(int nbrAnimaux) {
        this.nbrAnimaux = nbrAnimaux;
    }

    public void afficherZoo() {
        System.out.println("Nom: " + nom + ", Ville: " + ville + ", Nbr cages: " + NBR_CAGES);
    }

    public boolean ajouterAnimal(Animal animal) {
        if (isZooFull()) {
            return false;
        }
        if (rechercherAnimal(animal) != -1) {
            return false;
        }
        animaux[nbrAnimaux] = animal;
        nbrAnimaux++;
        return true;
    }

    public void afficherAnimaux() {
        for (int i = 0; i < nbrAnimaux; i++) {
            System.out.println(animaux[i]);
        }
    }

    public int rechercherAnimal(Animal animal) {
        for (int i = 0; i < nbrAnimaux; i++) {
            if (animaux[i].getNom().equals(animal.getNom())) {
                return i;
            }
        }
        return -1;
    }

    public boolean supprimerAnimal(Animal animal) {
        int index = rechercherAnimal(animal);
        if (index == -1) {
            return false;
        }
        for (int i = index; i < nbrAnimaux - 1; i++) {
            animaux[i] = animaux[i + 1];
        }
        animaux[nbrAnimaux - 1] = null;
        nbrAnimaux--;
        return true;
    }

    public boolean isZooFull() {
        return nbrAnimaux >= NBR_CAGES;
    }

    public static Zoo zooAvecPlusAnimaux(Zoo z1, Zoo z2) {
        if (z1.nbrAnimaux > z2.nbrAnimaux) {
            return z1;
        }
        return z2;
    }

    @Override
    public String toString() {
        return "Zoo [nom=" + nom + ", ville=" + ville + ", nbrAnimaux=" + nbrAnimaux + "]";
    }
}