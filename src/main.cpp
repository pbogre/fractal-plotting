#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

// TODO
// - make it run smoothly (async rendering) (try on windows)
// 	- how tf was pezza getting 1800 fps
// - better, smoother coloring
// - configs for julia / mandelbrot switching

// Z = Zn-1 ^ 2 + C
sf::Vector2f compute_next(sf::Vector2f z, sf::Vector2f c){
  // burning ship:
  //const float abs_x = abs(z.x);
  //const float abs_y = abs(z.y);
  //const float re = abs_x * abs_x - abs_y * abs_y;
  //const float im = 2 * abs_x * abs_y;
  
  const float re = z.x * z.x - z.y * z.y;
  const float im = z.x * z.y * 2;

  sf::Vector2f z_sqrd(re, im);
  return z_sqrd + c;
}

float mod(sf::Vector2f z){
  return sqrt(z.x*z.x + z.y*z.y);
}

float iterate(sf::Vector2f z0, sf::Vector2f c, int max_iterations){
  int iteration = 0;
  sf::Vector2f zn = z0;

  while(iteration < max_iterations && mod(zn) < 4){
    zn = compute_next(zn, c);
    iteration++;
  }

  // found this online lol
  const float smooth_iteration = (float)iteration - log2(max(1.0f, log2(mod(zn))));
  return smooth_iteration;
}

// redesign palette, palette.hpp?
// redesign index algorithm, use ratio iterations/max_iterations and assign color to some values 
// e.g. 0, 0.05, 0.10, ..., 0.95, 1
//
// currently adds 8.5% frame time
class PALETTE{
  const sf::Color colors[5] = {
    sf::Color (0, 0, 0),	// black
    sf::Color (47, 36, 64),	// purple haze
    sf::Color (186, 15, 48),	// burgundy
    sf::Color (255, 37, 17),	// red
    sf::Color (198, 183, 155)	// sand dollar
  };

  const int segments = (sizeof(colors) / sizeof(sf::Color)) - 1;
  float ranges[4]; // n = segments

  sf::Color color_multiply(sf::Color color, float constant){
    const int r = color.r * constant;
    const int g = color.g * constant;
    const int b = color.b * constant;

    return sf::Color(r,g,b);
  }

  public:
    // range_n = base ^ n * a1
    // where range_(segments) must = max_iterations
    // a1*base^0, a1*base^1, ..., 500
    //
    // base was chosen arbitrarily by experimentation
    PALETTE(int max_iterations){
      const float base     = 1.7;
      const float a1       = max_iterations * pow(base, -segments);

      for(int i = 0; i < segments; i++) ranges[i] = pow(base, i) * a1;
    }

    // algorithm to determine which threshold an iteration reaches
    float get_color_index(float iterations, int max_iterations){
      float index = 0;

      for(int i = 0; i < segments; i++){
	if(iterations - ranges[i] < 0){
	  index += iterations / ranges[i];
	  break;
	}

	index += 1;
      }

      return index;
    }

    sf::Color get_color(float index){
      const float pos = index - (int)floor(index);
      //1.3 -> 70%(1) + 30%(2)
      //should help smoothen the palette
      sf::Color first_component  = color_multiply(colors[(int)floor(index)], 1-pos);
      sf::Color second_component = color_multiply(colors[(int)ceil(index)], pos);

      return first_component + second_component;
    }
};


int main(){ 
    sf::RenderWindow window = sf::RenderWindow{ { 500, 500 }, "Mandelbrot Plotting" };

    const float half_width   = (float)window.getSize().x / 2;
    const float half_height  = (float)window.getSize().y / 2;
    const float window_ratio = (float)window.getSize().x / window.getSize().y;
    
    const int max_iterations = 200;

    float scale = 1;
    float x_shift = 0;
    float y_shift = 0;

    sf::Vector2f c(-1.75, 0);

    PALETTE palette(max_iterations);

    float time;
    chrono::high_resolution_clock::time_point start;
    chrono::high_resolution_clock::time_point end;
    while (window.isOpen())
    {
        for (sf::Event event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed){
                window.close();
            }
            if (event.type == sf::Event::MouseMoved) {
	      //float x = sf::Mouse::getPosition(window).x;
	      //float y = sf::Mouse::getPosition(window).y;

	      
	      //const float px = (x + x_shift - half_width) * scale / half_width;
	      //const float py = (-y -y_shift+ half_height) * scale / half_height;

	      //const float iterations = iterate(sf::Vector2f(0,0), sf::Vector2f(px, py), max_iterations);

	      //PALETTE palette(max_iterations);
	      //float index = palette.get_color_index(iterations, max_iterations);
	      //cout << iterations << " -> " << index << endl;
	    }
	    if(event.type == sf::Event::KeyPressed){
	      if(event.key.code == sf::Keyboard::Down)  c.y -= 0.01;
	      if(event.key.code == sf::Keyboard::Up)    c.y += 0.01;
	      if(event.key.code == sf::Keyboard::Left)  c.x -= 0.01;
	      if(event.key.code == sf::Keyboard::Right) c.x += 0.01;

	      if(event.key.code == sf::Keyboard::W)     y_shift -= scale / 3;
	      if(event.key.code == sf::Keyboard::S)     y_shift += scale / 3;
	      if(event.key.code == sf::Keyboard::D)     x_shift += scale / 3;
	      if(event.key.code == sf::Keyboard::A)     x_shift -= scale / 3;

	      if(event.key.code == sf::Keyboard::I)     scale *= 0.9;
	      if(event.key.code == sf::Keyboard::O)     scale *= 1.1;
	    }
        }

        window.clear();

	start = chrono::high_resolution_clock::now();

	sf::VertexArray arr;
	int i = 1;
	for(int x = 0; x < window.getSize().x; x++){
	  for(int y = 0; y < window.getSize().y; y++){
	    //i++;
	    //cout << "\r" << i << " / " << window.getSize().x * window.getSize().y;

	    const float px = ( x - half_width ) * 2 * scale / half_width  + x_shift;
	    const float py = (-y + half_height) * 2 * scale / half_height - y_shift;

	    //const float iterations = iterate(sf::Vector2f(px, py), c, max_iterations);
	    const float iterations = iterate(sf::Vector2f(0, 0), sf::Vector2f(px, py), max_iterations);
	    
	    sf::Vertex vertex(sf::Vector2f(x, y));
	    //vertex.color = palette.get_color(palette.get_color_index(iterations, max_iterations));
	    vertex.color = sf::Color(255, 255, 255, 5 * iterations);
	    arr.append(vertex);
	  }
	}

	window.draw(arr);
        window.display();

	end = chrono::high_resolution_clock::now();
	time = 1e-6*(float)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
	cout << time << "ms" << endl;
    }
}
