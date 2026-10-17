public class Main {
    public static void main(String[] args) {
        Animal lion = new Animal();
        lion.family = "Felidae";
        lion.name = "Simba";
        lion.age = 5;
        lion.isMammal = true;

        Zoo myZoo = new Zoo();
        myZoo.name = "Friguia Park";
        myZoo.city = "Boufiqra";
        myZoo.nbrCages = 30;

        Animal tiger = new Animal("Felidae", "Shere Khan", 7, true);
        Animal elephant = new Animal("Elephantidae", "Dumbo", 3, true);

        Zoo myNewZoo = new Zoo("Belvédère", "Tunis", 20);

        myZoo.displayZoo();

        System.out.println(myZoo);
        System.out.println(myZoo.toString());

        System.out.println(tiger);
        System.out.println(tiger.toString());
    }
}