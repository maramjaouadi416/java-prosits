
public class Animal {
    private String nom;
    private String famille;

    public Animal(String nom, String famille) {
        this.nom = nom;
        this.famille = famille;
    }

    public String getNom() {
        return nom;
    }

    public String getFamille() {
        return famille;
    }

    @Override
    public String toString() {
        return "Nom : " + nom + ", Famille : " + famille;
    }
}
