package tn.esprit.gestionzoo.entities;

public class Animal {
    private String famille;
    private String nom;
    private int age;
    private boolean isMammal;

    public Animal() {
    }

    public Animal(String famille, String nom, int age, boolean isMammal) {
        this.famille = famille;
        this.nom = nom;
        setAge(age);
        this.isMammal = isMammal;
    }

    public String getFamille() {
        return famille;
    }

    public void setFamille(String famille) {
        this.famille = famille;
    }

    public String getNom() {
        return nom;
    }

    public void setNom(String nom) {
        this.nom = nom;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        if (age < 0) {
            this.age = 0;
        } else {
            this.age = age;
        }
    }

    public boolean isMammal() {
        return isMammal;
    }

    public void setMammal(boolean mammal) {
        isMammal = mammal;
    }

    @Override
    public String toString() {
        return "Animal [famille=" + famille + ", nom=" + nom + ", age=" + age + ", isMammal=" + isMammal + "]";
    }
}