# Tasarım Kararları ve Akademik Gerekçeler
## Hastane Randevu Yönetim Sistemi - Final Proje Raporu

Bu bölüm, projede kullanılan veri yapıları ve tasarım kararlarının akademik gerekçelerini açıklamaktadır.

---

## AVL Tree Tasarım Gerekçesi

AVL Tree'nin bu projede kullanılmasının temel nedeni, randevu sistemlerinde zaman bazlı sorguların kritik öneme sahip olmasıdır. Normal bir Binary Search Tree (BST), sıralı veri girişinde O(n) yüksekliğe sahip olabilir ve bu durumda tüm işlemlerin zaman karmaşıklığı O(n)'e düşer. Ancak AVL Tree, otomatik dengeleme mekanizması sayesinde her zaman O(log n) yüksekliği garanti eder ve insert, search, delete işlemlerinin zaman karmaşıklığını O(log n) seviyesinde tutar.

Randevuların `start_time` değerine göre sıralanması, zaman aralığı sorgularını ve çakışma kontrolünü verimli hale getirir. Ağaç zaman bazlı sıralandığı için, belirli bir zaman aralığındaki randevuları bulmak için O(log n) zaman yeterlidir. Silme işleminde `appointment_id` kullanılması, kullanıcı dostu bir arayüz sağlar ancak bu durumda önce ID ile arama yapılması gerektiğinden worst-case O(n) karmaşıklığına yol açabilir. Bu trade-off, kullanıcı deneyimi açısından kabul edilebilir bir ödündür çünkü kullanıcıların randevu ID'sini bilmesi, zaman değerini bilmesinden daha pratiktir.

---

## Heap Tasarım Gerekçesi

Min-Heap'in kullanılması, en erken randevuyu bulma işlemini O(1) zamanda gerçekleştirmeyi mümkün kılar. Heap'in root düğümü her zaman minimum değeri (en erken zaman) içerdiğinden, `heap_peek_min()` fonksiyonu sabit zaman karmaşıklığına sahiptir. Bu özellik, randevu sistemlerinde "bir sonraki randevu" gibi sık yapılan sorgular için idealdir.

Heap'ten ID ile silme işleminin implement edilmemesi, tasarım karmaşıklığı ile performans arasındaki bir trade-off'tur. ID ile silme işlemi için heap'in tüm elemanlarını taramak gerekir ki bu O(n) zaman alır. Alternatif olarak, lazy deletion yaklaşımı kullanılabilir (silinen elemanları işaretlemek), ancak bu da heap'in boyutunu artırır ve bellek kullanımını olumsuz etkiler. Bu projede, randevu iptal işlemleri AVL Tree üzerinden gerçekleştirilir ve heap'ten silme yapılmaz. Bu yaklaşım, heap'in basitliğini korurken, iptal işlemlerinin AVL Tree üzerinden O(log n) karmaşıklığında yapılmasını sağlar. Heap'in temel amacı en erken randevuyu bulmak olduğundan, bu trade-off kabul edilebilirdir.

---

## Otomatik ID Üretimi Tasarım Gerekçesi

Otomatik ID üretimi, veri bütünlüğünü sağlamak ve kullanıcı hatalarını önlemek için tercih edilmiştir. Manuel ID girişi, duplicate ID'ler, geçersiz ID aralıkları ve ID çakışmaları gibi sorunlara yol açabilir. Sistem, CSV dosyalarından veri yüklendiğinde mevcut maksimum ID'leri tespit eder ve sayaçları buna göre günceller. Bu sayede, yeni eklenen kayıtların ID'leri her zaman mevcut maksimum ID'den büyük olur ve çakışma riski ortadan kalkar.

ID'lerin görünür ancak düzenlenemez (read-only) olması, kullanıcı deneyimi açısından önemlidir. Kullanıcılar, ekledikleri kayıtların ID'lerini görebilir ve bu ID'leri başka işlemlerde (örneğin randevu oluştururken hasta/doktor ID referansı) kullanabilirler. Ancak ID'lerin otomatik üretilmesi, veri bütünlüğünü garanti eder ve sistemin tutarlılığını korur. Bu yaklaşım, gerçek dünya uygulamalarında yaygın olarak kullanılan bir desendir ve veritabanı sistemlerinde de benzer şekilde uygulanır.

---

**Not:** Bu bölüm, üniversite final projesi raporu için hazırlanmıştır ve akademik bir ton kullanılmıştır. Raporunuza doğrudan kopyalayıp yapıştırabilirsiniz.

