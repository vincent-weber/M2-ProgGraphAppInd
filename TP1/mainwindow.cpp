#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::showSelections(MyMesh* _mesh)
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


    std::vector<float> t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
    float aire_limite1 = aire_min+ 0.1*(aire_max-aire_min);
    float aire_limite2 = aire_limite1+ 0.1*(aire_max-aire_min);
    float aire_limite3 = aire_limite2+ 0.1*(aire_max-aire_min);
    float aire_limite4 = aire_limite3+ 0.1*(aire_max-aire_min);
    float aire_limite5 = aire_limite4+ 0.1*(aire_max-aire_min);
    float aire_limite6 = aire_limite5+ 0.1*(aire_max-aire_min);
    float aire_limite7 = aire_limite6+ 0.1*(aire_max-aire_min);
    float aire_limite8 = aire_limite7+ 0.1*(aire_max-aire_min);
    float aire_limite9 = aire_limite8+ 0.1*(aire_max-aire_min);
    for(float i = 0 ; i <= aires.size(); i++)
    {
        if (aires[i] >= aire_min && aires[i] < aire_limite1) {
            t1.push_back(aires[i]);
        }
        else if (aires[i] >= aire_limite1 && aires[i] < aire_limite2) {
            t2.push_back(aires[i]);
        }
        else if (aires[i] >= aire_limite2 && aires[i] < aire_limite3) {
            t3.push_back(aires[i]);
        }else if (aires[i] >= aire_limite3 && aires[i] < aire_limite4) {
            t4.push_back(aires[i]);
        }else if (aires[i] >= aire_limite4 && aires[i] < aire_limite5) {
            t5.push_back(aires[i]);
        }else if (aires[i] >= aire_limite5 && aires[i] < aire_limite6) {
            t6.push_back(aires[i]);
        }else if (aires[i] >= aire_limite6 && aires[i] < aire_limite7) {
            t7.push_back(aires[i]);
        }else if (aires[i] >= aire_limite7 && aires[i] < aire_limite8) {
            t8.push_back(aires[i]);
        }else if (aires[i] >= aire_limite8 && aires[i] < aire_limite9) {
            t9.push_back(aires[i]);
        } else if (aires[i] >= aire_limite9 && aires[i] < aire_max) {
            t10.push_back(aires[i]);
        }
    }


    // on réinitialise les couleurs de tout le maillage
    /*resetAllColorsAndThickness(_mesh);
    qDebug() << "Nombre de sommets : " << _mesh->n_vertices();
    qDebug() << "Nombre d'arêtes : " << _mesh->n_edges();
    qDebug() << "Nombre de demi-arêtes : " << _mesh->n_halfedges();
    qDebug() << "Nombre de faces : " << _mesh->n_faces();

    if (vertexSelection > -1 && vertexSelection < _mesh->n_vertices()) {
        VertexHandle vh = _mesh->vertex_handle(vertexSelection);
        _mesh->set_color(vh, MyMesh::Color(255, 0, 0));
        _mesh->data(vh).thickness *= 10;
    }

    if (edgeSelection > -1 && edgeSelection < _mesh->n_edges()) {
        EdgeHandle eh = _mesh->edge_handle(edgeSelection);
        _mesh->set_color(eh, MyMesh::Color(0, 255, 0));
        _mesh->data(eh).thickness *= 10;
    }

    if (faceSelection > -1 && faceSelection < _mesh->n_faces()) {
        FaceHandle fh = _mesh->face_handle(faceSelection);
        _mesh->set_color(fh, MyMesh::Color(0, 0, 255));
    }

    // on affiche le nouveau maillage
    displayMesh(_mesh);*/
}





void MainWindow::showSelectionsNeighborhood(MyMesh* _mesh)
{
    // on réinitialise les couleurs de tout le maillage
    resetAllColorsAndThickness(_mesh);

    // circulate around the current vertex
    if (vertexSelection > -1 && vertexSelection < _mesh->n_vertices()) {
        VertexHandle vh = _mesh->vertex_handle(vertexSelection);
        _mesh->data(vh).thickness *= 10;
        for (MyMesh::VertexEdgeIter ve_it=mesh.ve_iter(vh); ve_it.is_valid(); ++ve_it) {
            _mesh->set_color(*ve_it, MyMesh::Color(255, 0, 0));
        }
    }

    if (edgeSelection > -1 && edgeSelection < _mesh->n_edges()) {
        EdgeHandle eh = _mesh->edge_handle(edgeSelection);
        _mesh->set_color(eh, MyMesh::Color(0, 255, 0));
        _mesh->data(eh).thickness *= 10;
        // parcours de toutes les faces, et des sommets de chaque face
        for (MyMesh::FaceIter curFace = _mesh->faces_begin(); curFace != _mesh->faces_end(); curFace++)
        {
            for (MyMesh::FaceEdgeIter curEdge = _mesh->fe_iter(*curFace); curEdge.is_valid(); curEdge++)
            {
                if (*curEdge == eh) {
                    _mesh->set_color(*curFace, MyMesh::Color(64, 255, 0));
                    continue;
                }
            }
        }
    }

    if (faceSelection > -1 && faceSelection < _mesh->n_faces()) {
        FaceHandle fh = _mesh->face_handle(faceSelection);
        _mesh->set_color(fh, MyMesh::Color(0, 0, 255));
        for (MyMesh::FaceFaceIter ff_it = mesh.ff_iter(fh) ; ff_it.is_valid() ; ++ff_it) {
            _mesh->set_color(*ff_it, MyMesh::Color(0, 64, 255));
        }
    }

    // on affiche le nouveau maillage
    displayMesh(_mesh);
}



void MainWindow::showBorder(MyMesh* _mesh)
{
    // on réinitialise l'affichage
    resetAllColorsAndThickness(_mesh);

    //is_boundary (EdgeHandle _eh)
    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        EdgeHandle eh = *curEdge;
        if (_mesh->is_boundary(eh)) {
            _mesh->set_color(eh, MyMesh::Color(255,0,255));
            _mesh->data(eh).thickness *= 10;
        }
    }

    // on affiche le nouveau maillage
    displayMesh(_mesh);
}


void MainWindow::showPath(MyMesh* _mesh, int v1, int v2)
{
    // on réinitialise l'affichage
    resetAllColorsAndThickness(_mesh);

    VertexHandle v1_h = _mesh->vertex_handle(v1);
    VertexHandle v2_h = _mesh->vertex_handle(v2);
    // point de départ et point d'arrivée en vert et en gros
    _mesh->set_color(v1_h, MyMesh::Color(0, 255, 0));
    _mesh->set_color(v2_h, MyMesh::Color(0, 255, 0));
    _mesh->data(_mesh->vertex_handle(v1)).thickness = 12;
    _mesh->data(_mesh->vertex_handle(v2)).thickness = 12;

    QVector<VertexHandle> handles;
    for (MyMesh::VertexVertexIter vv_it = mesh.vv_iter(v1_h) ; vv_it->idx() != v2 && vv_it.is_valid() ;) {
        while (handles.contains(vv_it))
            ++vv_it;
        _mesh->set_color(*vv_it, MyMesh::Color(0, 64, 255));
        _mesh->data(*vv_it).thickness = 12;
        handles.push_back(vv_it);
        vv_it = mesh.vv_iter(*vv_it);
        qDebug() << vv_it->idx();
    }
    _mesh->set_color(v1_h, MyMesh::Color(0, 255, 0));
    _mesh->set_color(v2_h, MyMesh::Color(0, 255, 0));

    /*HalfedgeHandle heh, heh_fin;
    heh_fin = _mesh.halfedge_handle(v2_h);
    heh = _mesh.halfedge_handle(v1_h);
    while(heh != heh_fin) {
            //heh = mesh.next_halfedge_handle(heh);
            _mesh->set_color(heh, MyMesh::Color(255,0,0));
    }*/

    // on affiche le nouveau maillage
    displayMesh(_mesh);
}

/* **** fin de la partie à compléter **** */


/* **** début de la partie boutons et IHM **** */

void MainWindow::on_pushButton_bordure_clicked()
{
    showBorder(&mesh);
}

void MainWindow::on_pushButton_voisinage_clicked()
{
    // changement de mode entre avec et sans voisinage
    if(modevoisinage)
    {
        ui->pushButton_voisinage->setText("Repasser en mode normal");
        modevoisinage = false;
    }
    else
    {
        ui->pushButton_voisinage->setText("Passer en mode voisinage");
        modevoisinage = true;
    }

    // on montre la nouvelle selection
    if(!modevoisinage)
        showSelections(&mesh);
    else
        showSelectionsNeighborhood(&mesh);
}


void MainWindow::on_pushButton_vertexMoins_clicked()
{
    // mise à jour de l'interface
    vertexSelection = vertexSelection - 1;
    ui->labelVertex->setText(QString::number(vertexSelection));

    // on montre la nouvelle selection
    if(!modevoisinage)
        showSelections(&mesh);
    else
        showSelectionsNeighborhood(&mesh);
}

void MainWindow::on_pushButton_vertexPlus_clicked()
{
    // mise à jour de l'interface
    vertexSelection = vertexSelection + 1;
    ui->labelVertex->setText(QString::number(vertexSelection));

    // on montre la nouvelle selection
    if(!modevoisinage)
        showSelections(&mesh);
    else
        showSelectionsNeighborhood(&mesh);
}

void MainWindow::on_pushButton_edgeMoins_clicked()
{
    // mise à jour de l'interface
    edgeSelection = edgeSelection - 1;
    ui->labelEdge->setText(QString::number(edgeSelection));

    // on montre la nouvelle selection
    if(!modevoisinage)
        showSelections(&mesh);
    else
        showSelectionsNeighborhood(&mesh);
}

void MainWindow::on_pushButton_edgePlus_clicked()
{
    // mise à jour de l'interface
    edgeSelection = edgeSelection + 1;
    ui->labelEdge->setText(QString::number(edgeSelection));

    // on montre la nouvelle selection
    if(!modevoisinage)
        showSelections(&mesh);
    else
        showSelectionsNeighborhood(&mesh);
}

void MainWindow::on_pushButton_faceMoins_clicked()
{
    // mise à jour de l'interface
    faceSelection = faceSelection - 1;
    ui->labelFace->setText(QString::number(faceSelection));

    // on montre la nouvelle selection
    if(!modevoisinage)
        showSelections(&mesh);
    else
        showSelectionsNeighborhood(&mesh);
}

void MainWindow::on_pushButton_facePlus_clicked()
{
    // mise à jour de l'interface
    faceSelection = faceSelection + 1;
    ui->labelFace->setText(QString::number(faceSelection));

    // on montre la nouvelle selection
    if(!modevoisinage)
        showSelections(&mesh);
    else
        showSelectionsNeighborhood(&mesh);
}

void MainWindow::on_pushButton_afficherChemin_clicked()
{
    // on récupère les sommets de départ et d'arrivée
    int indexV1 = ui->spinBox_v1_chemin->value();
    int indexV2 = ui->spinBox_v2_chemin->value();

    showPath(&mesh, indexV1, indexV2);
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
    vertexSelection = -1;
    edgeSelection = -1;
    faceSelection = -1;

    modevoisinage = false;

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

