#include "mainwindow.h"
#include "ui_mainwindow.h"

#define PI 3.14159265

void MainWindow::nbFacesSommetsAretes(MyMesh *_mesh) {
    qDebug() << "Nombre de faces : " << _mesh->n_faces();
    qDebug() << "Nombre d'aretes : " << _mesh->n_edges();
    qDebug() << "Nombre de sommets : " << _mesh->n_vertices();
}

void MainWindow::verifTopologie(MyMesh *_mesh) {
    bool facesQueTriangulaires = true;
    int nbFacesNonTriangulaires = 0;
    int nbFacesSeules = 0;
    int nbAretesSeules = 0;
    int nbSommetsSeuls = 0;

    //VERIF FACES
    for (MyMesh::FaceIter curFace = _mesh->faces_begin(); curFace != _mesh->faces_end(); curFace++) {
        int nbSommetsAdjacents = 0;

        for (MyMesh::FaceVertexIter curVert = _mesh->fv_iter(_mesh->face_handle(curFace->idx())); curVert.is_valid(); ++curVert, ++nbSommetsAdjacents);
        if (nbSommetsAdjacents != 3) {
            facesQueTriangulaires = false;
            ++nbFacesNonTriangulaires;
        }
        int nbFacesAdjacentes = 0;
        for (MyMesh::FaceFaceIter curFace2 = _mesh->ff_iter(*curFace) ; curFace2.is_valid() ; ++curFace2, ++nbFacesAdjacentes);
        if (nbFacesAdjacentes == 0) {
            ++nbFacesSeules;
        }
    }

    if (facesQueTriangulaires) {
        qDebug() << "Le maillage ne possède que des faces triangulaires.";
    } else {
        qDebug() << "Le maillage possède " << nbFacesNonTriangulaires << " faces non triangulaires.";
    }

    qDebug() << "Le maillage contient " << nbFacesSeules << " faces seules.";

    //VERIF ARETES
    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end() ; ++curEdge) {
        HalfedgeHandle h1 = _mesh->halfedge_handle(*curEdge, 0);
        FaceHandle f1 = _mesh->face_handle(h1);
        HalfedgeHandle h2 = _mesh->halfedge_handle(*curEdge, 1);
        FaceHandle f2 = _mesh->face_handle(h2);

        if (!f1.is_valid() && !f2.is_valid()) {
            ++nbAretesSeules;
        }
    }

    qDebug() << "Le maillage contient " << nbAretesSeules << " aretes seules.";

    //VERIF SOMMETS
    for (MyMesh::VertexIter curVert = _mesh->vertices_begin() ; curVert != _mesh->vertices_end() ; ++curVert) {
        int nbAretesAdjacentes = 0;
        for (MyMesh::VertexEdgeIter curEdge = _mesh->ve_iter(*curVert) ; curEdge.is_valid() ; ++curEdge, ++nbAretesAdjacentes);
        if (nbAretesAdjacentes == 0) {
            ++nbSommetsSeuls;
        }
    }

    qDebug() << "Le maillage contient " << nbSommetsSeuls << " sommets seuls.";
}


void MainWindow::boiteEnglobante(MyMesh *_mesh) {
    float minX = 0, maxX = 0, minY = 0, maxY = 0, minZ = 0, maxZ = 0;
    for (MyMesh::FaceIter curFace = _mesh->faces_begin(); curFace != _mesh->faces_end(); curFace++) {
        for (MyMesh::FaceVertexIter fv_it=mesh.fv_iter(*curFace); fv_it.is_valid(); ++fv_it) {
            if(_mesh->point(*fv_it)[0] < minX) minX = _mesh->point(*fv_it)[0];
            else if(_mesh->point(*fv_it)[0] > maxX) maxX = _mesh->point(*fv_it)[0];
            if(_mesh->point(*fv_it)[1] < minY) minY = _mesh->point(*fv_it)[1];
            else if(_mesh->point(*fv_it)[1] > maxY) maxY = _mesh->point(*fv_it)[1];
            if(_mesh->point(*fv_it)[2] < minZ) minZ = _mesh->point(*fv_it)[2];
            else if(_mesh->point(*fv_it)[2] > maxZ) maxZ = _mesh->point(*fv_it)[2];
        }
    }
    qDebug() << "Bounding box : x min " << minX << " max " << maxX
             << " y min " << minY << " max " << maxY
             << " z min " << minZ << " max " << maxZ;
}

void MainWindow::barycentre(MyMesh* _mesh) {
    double sumX = 0;
    double sumY = 0;
    double sumZ = 0;

    for (MyMesh::VertexIter v_it=_mesh->vertices_begin(); v_it!=_mesh->vertices_end(); ++v_it) {
        MyMesh::Point sommet_courant = _mesh->point(*v_it);
        sumX += sommet_courant[0];
        sumY += sommet_courant[1];
        sumZ += sommet_courant[2];
    }

    MyMesh::Point barycentre;
    barycentre[0] = sumX / _mesh->n_vertices();
    barycentre[1] = sumY / _mesh->n_vertices();
    barycentre[2] = sumZ / _mesh->n_vertices();
    qDebug() << "Barycentre de coordonnées : [ " << barycentre[0] << ", " << barycentre[1] << ", " << barycentre[2] << "]";

    qDebug() << "NB SOMMETS : " << _mesh->n_vertices();

    VertexHandle handle_bary = _mesh->add_vertex(barycentre);
    _mesh->set_color(handle_bary, MyMesh::Color(0, 255, 0));
    _mesh->data(handle_bary).thickness = 10;
    displayMesh(_mesh);

    qDebug() << "NB SOMMETS : " << _mesh->n_vertices();
}

void MainWindow::frequenceAires(MyMesh* _mesh)
{
    float aire_totale = 0;
    std::vector<float> aires;
    float aire_min = std::numeric_limits<float>::max();
    float aire_max = std::numeric_limits<float>::min();


    for (MyMesh::FaceIter curFace = _mesh->faces_begin(); curFace != _mesh->faces_end(); curFace++) {
        std::vector<MyMesh::Point> points;
        for (MyMesh::FaceVertexIter fv_it=mesh.fv_iter(*curFace); fv_it.is_valid(); ++fv_it) {
            points.push_back(_mesh->point(*fv_it));
        }
        MyMesh::Point p1 = points[0];
        MyMesh::Point p2 = points[1];
        MyMesh::Point p3 = points[2];

        VectorT<float,3> v12 = p2 - p1;
        VectorT<float,3> v13 = p3 - p1;

        VectorT<float,3> prod_vec = v12 % v13;
        float aire_triangle = 0.5f * (prod_vec.norm());
        aire_totale += aire_triangle;
        qDebug() << "Aire triangle en cours : " << aire_triangle;
        if (aire_triangle > aire_max) {
            aire_max = aire_triangle;
        }
        if (aire_triangle < aire_min) {
            aire_min = aire_triangle;
        }
        aires.push_back(aire_triangle);
    }
    qDebug() << "Aire totale finale : " << aire_totale;
    qDebug() << "Aire moyenne : " << aire_totale / _mesh->n_faces();
    qDebug() << "Aire min : " << aire_min;
    qDebug() << "Aire max : " << aire_max;


    unsigned int t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
    t1 = t2 = t3 = t4 = t5 = t6 = t7 = t8 = t9 = t10 = 0;
    float ecart = 0.1*(aire_max-aire_min);
    float aire_limite1 = aire_min + ecart;
    float aire_limite2 = aire_limite1 + ecart;
    float aire_limite3 = aire_limite2 + ecart;
    float aire_limite4 = aire_limite3 + ecart;
    float aire_limite5 = aire_limite4 + ecart;
    float aire_limite6 = aire_limite5 + ecart;
    float aire_limite7 = aire_limite6 + ecart;
    float aire_limite8 = aire_limite7 + ecart;
    float aire_limite9 = aire_limite8 + ecart;

    for(float i = 0 ; i <= aires.size(); i++)
    {
        if (aires[i] >= aire_min && aires[i] < aire_limite1) {
            t1++;
        } else if (aires[i] >= aire_limite1 && aires[i] < aire_limite2) {
            t2++;
        } else if (aires[i] >= aire_limite2 && aires[i] < aire_limite3) {
            t3++;
        } else if (aires[i] >= aire_limite3 && aires[i] < aire_limite4) {
            t4++;
        } else if (aires[i] >= aire_limite4 && aires[i] < aire_limite5) {
            t5++;
        } else if (aires[i] >= aire_limite5 && aires[i] < aire_limite6) {
            t6++;
        } else if (aires[i] >= aire_limite6 && aires[i] < aire_limite7) {
            t7++;
        } else if (aires[i] >= aire_limite7 && aires[i] < aire_limite8) {
            t8++;
        } else if (aires[i] >= aire_limite8 && aires[i] < aire_limite9) {
            t9++;
        } else if (aires[i] >= aire_limite9 && aires[i] <= aire_max) {
            t10++;
        }
    }

    qDebug() << "Nb Total Triangles : " << _mesh->n_faces();
    qDebug() << "Nb Triangles aire entre " << aire_min << " et " << aire_limite1 << " : " << t1;
    qDebug() << "Nb Triangles aire entre " << aire_limite1 << " et " << aire_limite2 << " : " << t2;
    qDebug() << "Nb Triangles aire entre " << aire_limite2 << " et " << aire_limite3 << " : " << t3;
    qDebug() << "Nb Triangles aire entre " << aire_limite3 << " et " << aire_limite4 << " : " << t4;
    qDebug() << "Nb Triangles aire entre " << aire_limite4 << " et " << aire_limite5 << " : " << t5;
    qDebug() << "Nb Triangles aire entre " << aire_limite5 << " et " << aire_limite6 << " : " << t6;
    qDebug() << "Nb Triangles aire entre " << aire_limite6 << " et " << aire_limite7 << " : " << t7;
    qDebug() << "Nb Triangles aire entre " << aire_limite7 << " et " << aire_limite8 << " : " << t8;
    qDebug() << "Nb Triangles aire entre " << aire_limite8 << " et " << aire_limite9 << " : " << t9;
    qDebug() << "Nb Triangles aire entre " << aire_limite9 << " et " << aire_max << " : " << t10;

    qDebug() << "Somme = " << t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8 + t9 + t10;
}

void MainWindow::valences(MyMesh *_mesh) {

}


void MainWindow::deviationNormales(MyMesh *_mesh) {

    for (MyMesh::FaceIter curFace = _mesh->faces_begin(); curFace != _mesh->faces_end(); curFace++) {
        if (curFace->idx() == -1) {
            qDebug() << "HIOHFIOEZIHFOEZIH";
        }
        _mesh->calc_face_normal(*curFace);
        _mesh->update_normal(*curFace);
    }

    for (MyMesh::VertexIter v_it=_mesh->vertices_begin(); v_it!=_mesh->vertices_end(); ++v_it) {
        MyMesh::Normal normalVertex = _mesh->calc_vertex_normal(*v_it);
        double angle_max = 0;
        for (MyMesh::VertexOHalfedgeIter he = _mesh->voh_iter(*v_it) ; he.is_valid() ; ++he) {
            //BUG ICI face.idx = -1 pour les maillages cowhead et hexagon
            FaceHandle face = _mesh->face_handle(*he);
            qDebug() << face.idx();
            MyMesh::Normal normalFace = _mesh->normal(face);
            double scal_prod = normalVertex.normalized() | normalFace.normalized();
            double angle = acos(scal_prod);
            if (abs(angle) > angle_max) {
                angle_max = angle;
            }



        }
        float color = angle_max * (255.0f / PI);
        _mesh->set_color(*v_it, MyMesh::Color(0, color, 0));
        _mesh->data(*v_it).thickness = 10;

    }
    displayMesh(_mesh);
}

void MainWindow::anglesDiedres(MyMesh *_mesh) {

}


/* **** début de la partie boutons et IHM **** */

void MainWindow::on_button_NbFAS_clicked()
{
    nbFacesSommetsAretes(&mesh);
}

void MainWindow::on_button_VerifTopo_clicked()
{
    verifTopologie(&mesh);
}

void MainWindow::on_button_BoiteEnglo_clicked()
{
    boiteEnglobante(&mesh);
}

void MainWindow::on_button_Barycentre_clicked()
{
    barycentre(&mesh);
}

void MainWindow::on_button_FreqAires_clicked()
{
    frequenceAires(&mesh);
}

void MainWindow::on_button_Valences_clicked()
{
    valences(&mesh);
}

void MainWindow::on_button_DeviaNormales_clicked()
{
    deviationNormales(&mesh);
}

void MainWindow::on_button_AnglesDiedres_clicked()
{
    anglesDiedres(&mesh);
}


void MainWindow::on_pushButton_chargement_clicked()
{
    // fenêtre de sélection des fichiers
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Mesh"), "", tr("Mesh Files (*.obj)"));

    // chargement du fichier .obj dans la variable globale "mesh"
    OpenMesh::IO::read_mesh(mesh, fileName.toUtf8().constData());

    // initialisation des couleurs et épaisseurs (sommets et arêtes) du mesh
    resetAllColorsAndThickness(&mesh);

    // on affiche le maillage
    displayMesh(&mesh);
}

/* **** fin de la partie boutons et IHM **** */



/* **** fonctions supplémentaires **** */

// permet d'initialiser les couleurs et les épaisseurs des élements du maillage
void MainWindow::resetAllColorsAndThickness(MyMesh* _mesh)
{
    for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
    {
        _mesh->data(*curVert).thickness = 1;
        _mesh->set_color(*curVert, MyMesh::Color(0, 0, 0));
    }

    for (MyMesh::FaceIter curFace = _mesh->faces_begin(); curFace != _mesh->faces_end(); curFace++)
    {
        _mesh->set_color(*curFace, MyMesh::Color(150, 150, 150));
    }

    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        _mesh->data(*curEdge).thickness = 1;
        _mesh->set_color(*curEdge, MyMesh::Color(0, 0, 0));
    }
}

// charge un objet MyMesh dans l'environnement OpenGL
void MainWindow::displayMesh(MyMesh* _mesh, DisplayMode mode)
{
    GLuint* triIndiceArray = new GLuint[_mesh->n_faces() * 3];
    GLfloat* triCols = new GLfloat[_mesh->n_faces() * 3 * 3];
    GLfloat* triVerts = new GLfloat[_mesh->n_faces() * 3 * 3];

    int i = 0;

    if(mode == DisplayMode::TemperatureMap)
    {
        QVector<float> values;
        for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
            values.append(fabs(_mesh->data(*curVert).value));
        qSort(values);

        float range = values.at(values.size()*0.8);

        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;

        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }

    if(mode == DisplayMode::Normal)
    {
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;
        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }

    if(mode == DisplayMode::ColorShading)
    {
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;
        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            triCols[3*i+0] = _mesh->data(*fvIt).faceShadingColor[0]; triCols[3*i+1] = _mesh->data(*fvIt).faceShadingColor[1]; triCols[3*i+2] = _mesh->data(*fvIt).faceShadingColor[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->data(*fvIt).faceShadingColor[0]; triCols[3*i+1] = _mesh->data(*fvIt).faceShadingColor[1]; triCols[3*i+2] = _mesh->data(*fvIt).faceShadingColor[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->data(*fvIt).faceShadingColor[0]; triCols[3*i+1] = _mesh->data(*fvIt).faceShadingColor[1]; triCols[3*i+2] = _mesh->data(*fvIt).faceShadingColor[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }


    ui->displayWidget->loadMesh(triVerts, triCols, _mesh->n_faces() * 3 * 3, triIndiceArray, _mesh->n_faces() * 3);

    delete[] triIndiceArray;
    delete[] triCols;
    delete[] triVerts;

    GLuint* linesIndiceArray = new GLuint[_mesh->n_edges() * 2];
    GLfloat* linesCols = new GLfloat[_mesh->n_edges() * 2 * 3];
    GLfloat* linesVerts = new GLfloat[_mesh->n_edges() * 2 * 3];

    i = 0;
    QHash<float, QList<int> > edgesIDbyThickness;
    for (MyMesh::EdgeIter eit = _mesh->edges_begin(); eit != _mesh->edges_end(); ++eit)
    {
        float t = _mesh->data(*eit).thickness;
        if(t > 0)
        {
            if(!edgesIDbyThickness.contains(t))
                edgesIDbyThickness[t] = QList<int>();
            edgesIDbyThickness[t].append((*eit).idx());
        }
    }
    QHashIterator<float, QList<int> > it(edgesIDbyThickness);
    QList<QPair<float, int> > edgeSizes;
    while (it.hasNext())
    {
        it.next();

        for(int e = 0; e < it.value().size(); e++)
        {
            int eidx = it.value().at(e);

            MyMesh::VertexHandle vh1 = _mesh->to_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
            linesVerts[3*i+0] = _mesh->point(vh1)[0];
            linesVerts[3*i+1] = _mesh->point(vh1)[1];
            linesVerts[3*i+2] = _mesh->point(vh1)[2];
            linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
            linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
            linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
            linesIndiceArray[i] = i;
            i++;

            MyMesh::VertexHandle vh2 = _mesh->from_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
            linesVerts[3*i+0] = _mesh->point(vh2)[0];
            linesVerts[3*i+1] = _mesh->point(vh2)[1];
            linesVerts[3*i+2] = _mesh->point(vh2)[2];
            linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
            linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
            linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
            linesIndiceArray[i] = i;
            i++;
        }
        edgeSizes.append(qMakePair(it.key(), it.value().size()));
    }

    ui->displayWidget->loadLines(linesVerts, linesCols, i * 3, linesIndiceArray, i, edgeSizes);

    delete[] linesIndiceArray;
    delete[] linesCols;
    delete[] linesVerts;

    GLuint* pointsIndiceArray = new GLuint[_mesh->n_vertices()];
    GLfloat* pointsCols = new GLfloat[_mesh->n_vertices() * 3];
    GLfloat* pointsVerts = new GLfloat[_mesh->n_vertices() * 3];

    i = 0;
    QHash<float, QList<int> > vertsIDbyThickness;
    for (MyMesh::VertexIter vit = _mesh->vertices_begin(); vit != _mesh->vertices_end(); ++vit)
    {
        float t = _mesh->data(*vit).thickness;
        if(t > 0)
        {
            if(!vertsIDbyThickness.contains(t))
                vertsIDbyThickness[t] = QList<int>();
            vertsIDbyThickness[t].append((*vit).idx());
        }
    }
    QHashIterator<float, QList<int> > vitt(vertsIDbyThickness);
    QList<QPair<float, int> > vertsSizes;

    while (vitt.hasNext())
    {
        vitt.next();

        for(int v = 0; v < vitt.value().size(); v++)
        {
            int vidx = vitt.value().at(v);

            pointsVerts[3*i+0] = _mesh->point(_mesh->vertex_handle(vidx))[0];
            pointsVerts[3*i+1] = _mesh->point(_mesh->vertex_handle(vidx))[1];
            pointsVerts[3*i+2] = _mesh->point(_mesh->vertex_handle(vidx))[2];
            pointsCols[3*i+0] = _mesh->color(_mesh->vertex_handle(vidx))[0];
            pointsCols[3*i+1] = _mesh->color(_mesh->vertex_handle(vidx))[1];
            pointsCols[3*i+2] = _mesh->color(_mesh->vertex_handle(vidx))[2];
            pointsIndiceArray[i] = i;
            i++;
        }
        vertsSizes.append(qMakePair(vitt.key(), vitt.value().size()));
    }

    ui->displayWidget->loadPoints(pointsVerts, pointsCols, i * 3, pointsIndiceArray, i, vertsSizes);

    delete[] pointsIndiceArray;
    delete[] pointsCols;
    delete[] pointsVerts;
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
