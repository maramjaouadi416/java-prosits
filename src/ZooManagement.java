import java.util.Scanner;
public class ZooManagement {
    public static void main(String[] args) {
        int nbrCages = 20;
        String zooName = "my zoo";
        // System.out.println(zooName + " comporte " + nbrCages + " cages ");
        Scanner sc = new Scanner(System.in);
        // System.out.println("veuillez entrer le nom du zoo");
        // zooName=sc.nextLine();
        //System.out.println("veuillez entrer le nombre des cages");
        //nbrCages=sc.nextInt();
        //System.out.println(zooName + " comporte " + nbrCages + " cages ");
        do {
            System.out.println("veuillez entrer le nom du zoo");
            zooName=sc.nextLine();
            if (zooName.isEmpty()) {
                System.out.println("veuillez entrer un nom valide");
            }
        } while (zooName.isEmpty());
        do{
            System.out.println("veuillez entrer le nombre des cages");
            nbrCages=sc.nextInt();
            if (nbrCages <= 0){
                System.out.println("le nombre des cages doit etre positif");
            }
        } while (nbrCages <= 0);
        System.out.println(zooName + " comporte " + nbrCages + " cages ");
        sc.close();
    }




    
}
